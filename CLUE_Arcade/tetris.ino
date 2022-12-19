#ifndef POSIX_BUILD
# include <FastLED.h>
# include <EEPROM.h>
#endif
#include "src/tetris.h"

#define ROWS 20
#define COLUMNS 10
int field[ROWS][COLUMNS] = { 0 };

#define PWIDTH 4
#define PHEIGHT 4
struct piece {
  int origin_x;
  int origin_y;
  int rotation;
  int type;
  int squares[PHEIGHT][PWIDTH];
};
int combo = 1;
int lines_cleared = 0;
int level = 0;

bool GameOver_tetris;

long int last_tick;

// forward declaration

struct piece active_piece;
struct piece next_piece;

#define PTYPES 7
char piece_shapes[PTYPES][PHEIGHT][PWIDTH * 4 + 1] = {
  { " #       #      ",
    " #  #### #  ####",
    " #       #      ",
    " #       #      "
  },

  { " ##  ##  ##  ## ",
    " ##  ##  ##  ## ",
    "                ",
    "                "
  },

  { "##   #  ##   #  ",
    " ## ##   ## ##  ",
    "    #       #   ",
    "                "
  },

  { " ##  #   ##  #  ",
    "##   ## ##   ## ",
    "      #       # ",
    "                "
  },

  { " #   #       #  ",
    "###  ## ### ##  ",
    "     #   #   #  ",
    "                "
  },

  { "  #  #      ##  ",
    "###  #  ###  #  ",
    "     ## #    #  ",
    "                "
  },

  { "#    ##      #  ",
    "###  #  ###  #  ",
    "     #    # ##  ",
    "                "
  },
};

// used by console rendering
char * field_colors[PTYPES + 1] = {
  (char *)"\e[0;30m",
  (char *)"\e[0;31m",
  (char *)"\e[0;32m",
  (char *)"\e[0;33m",
  (char *)"\e[0;34m",
  (char *)"\e[0;35m",
  (char *)"\e[0;36m",
  (char *)"\e[1;37m",
};

void get_shape(struct piece * p, int type, int rotation)
{
  for (int y = 0; y < PHEIGHT; ++y) {
    for (int x = 0; x < PWIDTH; ++x) {
      if ('#' == piece_shapes[type][y][rotation * 4 + x]) {
        p->squares[y][x] = type + 1;
      } else {
        p->squares[y][x] = 0;
      }
    }
  }
}

// XXX when a turn would hit square
// does it still rotate and push the piece to the side?
// nah. not in first impl.
void rotate_piece(struct piece * p, int steps)
{
  struct piece original_piece = *p;
  p->rotation += 4 + steps;
  p->rotation = active_piece.rotation % 4;
  get_shape(p, p->type, p->rotation);
  if (!try_move_piece(p, 0, 0)) {
    *p = original_piece;
  }
}

void new_piece(struct piece * new_p)
{
  struct piece p;
  p.origin_x = 3;
  p.origin_y = 0;
  p.rotation = 0;
  p.type = random() % PTYPES;
  get_shape(&p, p.type, p.rotation);
  *new_p = next_piece;
  next_piece = p;
}

void print_piece(struct piece * p)
{
  for (int i = 0; i < PHEIGHT; ++i) {
    printf("%01x%01x%01x%01x\n", p->squares[i][0], p->squares[i][1], p->squares[i][2], p->squares[i][3]);
  }
}

void render(void)
{
  clearScreen();
  //printf("\e[2J");
  for (int y = 0; y < ROWS; ++y) {
    // printf("\e[0m%-3d |", y);
    for (int x = 0; x < COLUMNS; ++x) {
      int field_val = field[y][x];
      int p_yofs = y - active_piece.origin_y ;
      int p_xofs = x - active_piece.origin_x ;
      if (0 <= p_yofs && p_yofs < PHEIGHT
          && 0 <= p_xofs && p_xofs < PWIDTH) {
        int p_val = active_piece.squares[p_yofs][p_xofs];
        if (p_val != 0) {
          field_val = p_val;
        }
      }
#ifndef POSIX_BUILD
      CRGB color;
      switch (field_val) {
        case 0: color = CRGB::Black; break;
        case 1: color = CRGB::Red; break;
        case 2: color = CRGB::Blue; break;
        case 3: color = CRGB::Yellow; break;
        case 4: color = CRGB::Purple; break;
        case 5: color = CRGB::ForestGreen; break;
        case 6: color = CRGB::Green; break;
        case 7: color = CRGB::Brown; break;
        case 8: color = CRGB::Grey; break;
      }

      for (int y = 18; y < 60; y++) {
        leds[XY(4, y)] = 0x080808;
        leds[XY(25, y)] = 0x080808;
      }
      for (int x = 4; x < 26; x++) {
        leds[XY(x, 17)] = 0x080808;
      }
      leds[XY(24 - x * 2, 19 + y * 2)] = color;
      leds[XY(24 - x * 2, 19 + y * 2 - 1)] = color;
      leds[XY(23 - x * 2, 19 + y * 2)] = color;
      leds[XY(23 - x * 2, 19 + y * 2 - 1)] = color;

      if (0) {
        leds[XY(29 - x * 3, y * 3)] = color;
        leds[XY(29 - x * 3, y * 3 - 1)] = color;
        leds[XY(29 - x * 3, y * 3 - 2)] = color;
        leds[XY(28 - x * 3, y * 3)] = color;
        leds[XY(28 - x * 3, y * 3 - 1)] = color;
        leds[XY(28 - x * 3, y * 3 - 2)] = color;
        leds[XY(27 - x * 3, y * 3)] = color;
        leds[XY(27 - x * 3, y * 3 - 1)] = color;
        leds[XY(27 - x * 3, y * 3 - 2)] = color;
      }
#endif

#ifdef POSIX_BUILD
      printf("%s%s", field_colors[field_val], "X");
#endif
    }
#ifdef POSIX_BUILD
    printf("\e[0m|\n");
#endif
  }
#ifndef POSIX_BUILD
  unsigned int tmp_score = score;
  unsigned int tmp_hs = read_high_score_top("t");
  unsigned int tmp_level = level;
  unsigned int tmp_combo = combo;
  //orange ff7300
//dark green 0caf00
  int green = 0x0caf00;
  int orange = 0xff7300;
  draw_color_xpm(orange, numbers[tmp_score % 10],3,6);
  draw_color_xpm(orange, numbers[(tmp_score/10) % 10],7,6);
  draw_color_xpm(orange, numbers[(tmp_score/100) % 10],11,6);
  draw_color_xpm(orange, numbers[(tmp_score/1000) % 10],15,6);
  draw_color_xpm(orange, numbers[(tmp_score/10000) % 10],19,6);



  draw_color_xpm(green, numbers[tmp_hs % 10],3,0);
  draw_color_xpm(green, numbers[(tmp_hs/10) % 10],7,0);
  draw_color_xpm(green, numbers[(tmp_hs/100) % 10],11,0);
  draw_color_xpm(green, numbers[(tmp_hs/1000) % 10],15,0);
  draw_color_xpm(green, numbers[(tmp_hs/10000) % 10],19,0);

  draw_color_xpm(orange, numbers[tmp_level % 10], 29, 53);
  draw_color_xpm(orange, numbers[(tmp_level/10) % 10], 29, 47);

  draw_color_xpm(green, numbers[(tmp_combo - 1) % 10], 3, 53);

  // draw next piece
  for (int y = 0; y < PHEIGHT; ++y) {
    for (int x = 0; x < PWIDTH; ++x) {
      int field_val = next_piece.squares[y][x];
      if (next_piece.squares[y][x] != 0) {
        CRGB color;
        switch (field_val) {
          case 0: color = CRGB::Black; break;
          case 1: color = CRGB::Red; break;
          case 2: color = CRGB::Blue; break;
          case 3: color = CRGB::Yellow; break;
          case 4: color = CRGB::Purple; break;
          case 5: color = CRGB::ForestGreen; break;
          case 6: color = CRGB::Green; break;
          case 7: color = CRGB::Brown; break;
          case 8: color = CRGB::Grey; break;
        }
        leds[XY(27 - x * 2, 1 + y * 2)]     = color;
        leds[XY(27 - x * 2, 1 + y * 2 + 1)] = color;
        leds[XY(26 - x * 2, 1 + y * 2)]     = color;
        leds[XY(26 - x * 2, 1 + y * 2 + 1)] = color;
      }
    }
  }
  FastLED.show();
#endif

#ifdef POSIX_BUILD
  printf("Score: %d\n", score);
#endif
}

void stick_piece(struct piece * p)
{ for (int y = 0; y < PHEIGHT; ++y) {
    for (int x = 0; x < PWIDTH; ++x) {
      // square is not used within piece. ignore
      if (p->squares[y][x] == 0) {
        continue;
      }
      // is square x,y plus delta occupied within field?
      field[p->origin_y + y][p->origin_x + x] = p->squares[y][x];
    }
  }
}

void remove_filled_rows()
{
  int temp_score = 0;
  for (int row = 0; row < ROWS; ++row) {
    int filled = 1;
    for (int x = 0; x < COLUMNS; ++x) {
      if (field[row][x] == 0) {
        filled = 0;
      }
    }
    if (filled) {
      ++temp_score;
      ++lines_cleared;
      if (lines_cleared % 3 == 0){
        if (speed_delay > 100)
        {
          level++;
          speed_delay *= 0.85;
        }
      }
      for (int move_row = row; move_row > 0; --move_row) {
        //memcpy(&field[move_row][0], &field[move_row - 1][0], 10);
        for (int move_col = 0; move_col < COLUMNS; ++move_col) {
          field[move_row][move_col] = field[move_row - 1][move_col];
        }
      }
    }
  }

  if (temp_score == 1) score += 1*(level+1)*combo;
  else if (temp_score == 2) score += 2*(level+1)*combo;
  else if (temp_score == 3) score += 7*(level+1)*combo;
  else if (temp_score == 4) score += 30*(level+1)*combo;

  if (temp_score >= 1){
    ++combo;
  }
  else combo = 1;
}

int try_move_piece(struct piece * p, int dy, int dx)
{
  for (int y = 0; y < PHEIGHT; ++y) {
    for (int x = 0; x < PWIDTH; ++x) {
      // square is not used within piece. ignore
      if (p->squares[y][x] == 0) {
        continue;
      }
      // is square x,y plus delta occupied within field?
      int wanted_ypos = p->origin_y + y + dy;
      int wanted_xpos = p->origin_x + x + dx;
      if (0 > wanted_ypos || wanted_ypos >= ROWS) {
        return 0;
      }
      if (0 > wanted_xpos || wanted_xpos >= COLUMNS) {
        return 0;
      }

      // is square x,y plus delta already occupied in field?
      if (field[wanted_ypos][wanted_xpos] != 0) {
        return 0;
      }
    }
  }
  p->origin_y += dy;
  p->origin_x += dx;
  return 1;
}

void tetris_setup()
{
  for (int y = 0; y < ROWS; ++y) {
    // printf("\e[0m%-3d |", y);
    for (int x = 0; x < COLUMNS; ++x) {
      field[y][x] = 0;
    }
  }
  score = 0;
  combo = 1;
  level = 0;
  speed_delay = 1000;
  last_tick = millis();
  new_piece(&active_piece);
  new_piece(&active_piece);
  render();
  GameOver = false;
  GameOver_tetris = false;
}

void tetris_loop() {
  long int current_time;
  if (GameOver_tetris) {
    if (score >  read_high_score_low("t")) {
      struct hs_player player;
      new_hs_player(score, player);
      write_high_score("t", player, curr_tetris_board);
      hsAnimation();
    }
    deathAnimation();

    GameOver = true;
    last_tick = millis();
    new_piece(&active_piece);
    render();
    switch_screen(&home_screen);
    return;
  }
  int action = check_joystick_input();
  current_time = millis();
   if (current_time - last_tick > speed_delay) {
      action |= ACT_D;
      last_tick = millis();
    }

  if (active_player == 1){
    if (action & ACT_L_P1){
      action = ACT_L;
    } else if (action & ACT_R_P1) {
      action = ACT_R;
    } else if (action & ACT_D_P1) {
      action = ACT_D;
    } else if (action & ACT_U_P1) {
      action = ACT_U;
    }
  }
  else{
    if (action & ACT_L_P2){
      action = ACT_L;
    } else if (action & ACT_R_P2) {
      action = ACT_R;
    } else if (action & ACT_D_P2) {
      action = ACT_D;
    } else if (action & ACT_U_P2) {
      action = ACT_U;
    }
  }

  if (action == ACT_NONE) {
    //return;
  }
  if (action & ACT_L) {
    try_move_piece(&active_piece, 0, -1);
  }
  if (action & ACT_R) {
    try_move_piece(&active_piece, 0, 1);
  }
  if (action & ACT_U) {
    rotate_piece(&active_piece, 1);
  }
  if (action & ACT_RN) {
    rotate_piece(&active_piece, -1);
  }
  if (action & ACT_D) {
    if (!try_move_piece(&active_piece, 1, 0)) {
      stick_piece(&active_piece);
      remove_filled_rows();
      new_piece(&active_piece);
      if (0 == try_move_piece(&active_piece, 0, 0)) {
        // game over!
        GameOver_tetris = true;
      }
    }
    last_tick = millis();
  }
  if (!GameOver) {
    //draw_plasma();
    render();
  }
}

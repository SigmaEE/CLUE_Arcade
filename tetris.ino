#ifndef POSIX_BUILD
# include <FastLED.h>
# include <EEPROM.h>
#endif

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

long int last_tick;

// forward declaration
int try_move_piece(struct piece * p, int dy, int dx);

struct piece active_piece;

#define PTYPES 7
char piece_shapes[PTYPES][PHEIGHT][PWIDTH * 4] = {
  { " #       #      ",
    " #  #### #  ####",
    " #       #      ",
    " #       #      "
  },

  { "##  ##  ##  ##  ",
    "##  ##  ##  ##  ",
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
  "\e[0;30m",
  "\e[0;31m",
  "\e[0;32m",
  "\e[0;33m",
  "\e[0;34m",
  "\e[0;35m",
  "\e[0;36m",
  "\e[1;37m",
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

void new_piece(struct piece * p)
{
  p->origin_x = 3;
  p->origin_y = 0;
  p->rotation = 0;
  p->type = random() % PTYPES;
  get_shape(p, p->type, p->rotation);
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

      leds[XY(29 - x * 3, y * 3)] = color;
      leds[XY(29 - x * 3, y * 3 - 1)] = color;
      leds[XY(29 - x * 3, y * 3 - 2)] = color;
      leds[XY(28 - x * 3, y * 3)] = color;
      leds[XY(28 - x * 3, y * 3 - 1)] = color;
      leds[XY(28 - x * 3, y * 3 - 2)] = color;
      leds[XY(27 - x * 3, y * 3)] = color;
      leds[XY(27 - x * 3, y * 3 - 1)] = color;
      leds[XY(27 - x * 3, y * 3 - 2)] = color;
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
  unsigned int tmp = score;
  int i = 0;
  while (tmp > 0) {
    if (tmp & 1) {
      leds[XY(0, i)] = CRGB::Blue;
    } else {
      leds[XY(0, i)] = CRGB::Red;
    }
    i++;
    tmp = tmp >> 1;
  }
  tmp = highScore_tetris;
  i = 0;
  while (tmp > 0) {
    if (tmp & 1) {
      leds[XY(29, i)] = CRGB::Blue;
    } else {
      leds[XY(29, i)] = CRGB::Red;
    }
    i++;
    tmp = tmp >> 1;
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
  for (int row = 0; row < ROWS; ++row) {
    int filled = 1;
    for (int x = 0; x < COLUMNS; ++x) {
      if (field[row][x] == 0) {
        filled = 0;
      }
    }
    if (filled) {
      ++score;
      //++count;
      if (score % 3 == 0)
      {
        if (speed_delay > 100)
        {
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
  speed_delay = 1000;
  last_tick = millis();
  new_piece(&active_piece);
  render();
  GameOver = false;
  GameOver_tetris = false;
}


void tetris_loop() {
  long int current_time;
  if (GameOver_tetris) {
    if (score > highScore_tetris) {
      highScore_tetris = score;
      writeHighScore();
      hsAnimation();
    }
    deathAnimation();

    GameOver = true;
    //return;
    //GameOver = false;
    last_tick = millis();
    new_piece(&active_piece);
    render();
    return;
  }
  int action = check_input();
  current_time = millis();

  if (current_time - last_tick > speed_delay) {
    action |= ACT_D;
    last_tick = millis();
  }

  if (action == ACT_NONE) {
    return;
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
    render();
  }
}

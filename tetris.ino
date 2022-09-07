#define ARDUINO 1

#ifdef ARDUINO
# include <FastLED.h>
# include <EEPROM.h>
#endif

#ifndef ARDUINO
# define CONSOLE_INPUT

# include <time.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/ioctl.h>
# include <termios.h>
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

// forward declarations
int try_move_piece(struct piece * p, int dy, int dx);

struct piece active_piece;
struct piece temp_piece;
struct piece default_piece = { 3, 0, 0 };

#define PTYPES 7
char piece_shapes[PTYPES][PHEIGHT][PWIDTH * 4] = {
  { " #       #      ",
    " #  #### #  ####",
    " #       #      ",
    " #       #      " },

  { "##  ##  ##  ##  ",
    "##  ##  ##  ##  ",
    "                ",
    "                " },

  { "##   #  ##   #  ",
    " ## ##   ## ##  ",
    "    #       #   ",
    "                " },

  { " ##  #   ##  #  ",
    "##   ## ##   ## ",
    "      #       # ",
    "                " },

  { " #   #       #  ",
    "###  ## ### ##  ",
    "     #   #   #  ",
    "                " },

  { "  #  #      ##  ",
    "###  #  ###  #  ",
    "     ## #    #  ",
    "                " },

  { "#    ##      #  ",
    "###  #  ###  #  ",
    "     #    # ##  ",
    "                " },
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
  temp_piece = *p;
  p->rotation += 4 + steps;
  p->rotation = active_piece.rotation % 4;
  get_shape(p, p->type, p->rotation);
  if (!try_move_piece(p, 0, 0)) {
    *p = temp_piece;
  }
}

void new_piece(struct piece * p)
{
  *p = default_piece;
  p->type = random() % PTYPES;
  get_shape(p, p->type, p->rotation);
}

#define ACT_NONE 0
#define ACT_L  (1 << 0)
#define ACT_R  (1 << 1)
#define ACT_D  (1 << 2)
#define ACT_RP (1 << 3)
#define ACT_RN (1 << 4)
#define ACT_Q  (1 << 5)

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
      
      leds[XY(29-x*3,y*3)] = color;
      leds[XY(29-x*3,y*3-1)] = color;
      leds[XY(29-x*3,y*3-2)] = color;
      leds[XY(28-x*3,y*3)] = color;
      leds[XY(28-x*3,y*3-1)] = color;
      leds[XY(28-x*3,y*3-2)] = color;
      leds[XY(27-x*3,y*3)] = color;
      leds[XY(27-x*3,y*3-1)] = color;
      leds[XY(27-x*3,y*3-2)] = color;
    //  printf("%s%s", field_colors[field_val], "X");
    }
   // printf("\e[0m|\n");
  }
  unsigned int tmp = score;
  int i = 0;
  while (tmp > 0) {
    if (tmp & 1) {
      leds[XY(0,i)] = CRGB::Blue;
    } else {
      leds[XY(0,i)] = CRGB::Red;
    }
    i++;
    tmp = tmp >> 1;
  }
  tmp = highScore;
  i = 0;
  while (tmp > 0) {
    if (tmp & 1) {
      leds[XY(29,i)] = CRGB::Blue;
    } else {
      leds[XY(29,i)] = CRGB::Red;
    }
    i++;
    tmp = tmp >> 1;
  }
  //printf("Score: %d\n", score);
  FastLED.show();
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
      ++count;
      if (score % 3 == 0)
      {
        if (speed_delay > 100)
        {
          speed_delay -= 100;
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

int check_input()
{
  int action = ACT_NONE;

#ifdef CONSOLE_INPUT
  long int n = 0;
  // if n == 0 there is nothing to read
  ioctl(STDIN_FILENO, FIONREAD, &n);
  while (n > 0) {
    int c = fgetc(stdin);
    switch (c) {
      case '1': action |= ACT_L; break;
      case '2': action |= ACT_R; break;
      case '3': action |= ACT_D; break;
      case '8': action |= ACT_RN; break;
      case '9': action |= ACT_RP; break;
      case 'q': action |= ACT_Q; break;
    }
    ioctl(STDIN_FILENO, FIONREAD, &n);
  }
#endif

  static long int current_time;
  static long int last_rot = 0;
  static long int last_down = 0;
  static long int last_left = 0;
  static long int last_right = 0;
  xPotVal = analogRead(xPotPin);
  yPotVal = analogRead(yPotPin);

  Serial.print("xval = ");
  Serial.println(xPotVal);
  Serial.print("yval = ");
  Serial.println(yPotVal);

  current_time = millis();

  if ( xPotVal < 400) {
    if (current_time - last_left > 100) {
      action |= ACT_L;
      last_left = millis();    
    }
  } else if (xPotVal > 600) {
    if (current_time - last_right > 100) {
      action |= ACT_R;
      last_right = millis();
    }
  }
  if (yPotVal < 400) {
    if (current_time - last_down > 50) {
      action |= ACT_D;
      last_down = millis();
    }
  } else if (yPotVal > 600) {
    
    if (current_time - last_rot > 300) {
        action |= ACT_RP;
        last_rot = millis();
    }
  }
  return action;
  // compare current input to new input.
  // if new input then use it
  // if repeating input (holds left, right or down)
  // then dont use it until after some time has passed
}

/*
void test_game(void)
{
  for (int i = 0; i < 1; ++i) {
    try_move_piece(&active_piece, 0, (random() % 3) - 1);
    rotate_piece(&active_piece, (random() % 3) - 1);
    if (0 == try_move_piece(&active_piece, 1, 0)) {
      stick_piece(&active_piece);
      new_piece(&active_piece);
      if (0 == try_move_piece(&active_piece, 0, 0)) {
        // game over!
        printf("Game over after %d moves\n", i);
        break;
      }
    }
    render();
    usleep(100000);
  }
}
*/

#ifdef POSIX_TIME
int time_delta(struct timespec * t1, struct timespec * t2)
{
  // return delta in milliseconds
  int delta = 0;
  delta = 1000 * (t2->tv_sec - t1->tv_sec);
        delta += (t2->tv_nsec - t1->tv_nsec) / (1000 * 1000);;
  return delta;
}
#endif

int tetris_gameover = 0;
long int last_tick;

void tetris_setup()
{
  last_tick = millis();
  new_piece(&active_piece);
  render();
}

void tetris_loop() {
    long int current_time;
    if (tetris_gameover) {
      if (score > highScore) {
        highScore = score;
        writeHighScore();
        hsAnimation();
      }
      deathAnimation();  
      for (int y = 0; y < ROWS; ++y) {
        // printf("\e[0m%-3d |", y);
        for (int x = 0; x < COLUMNS; ++x) {
          field[y][x] = 0;
        }
      }
      score = 0;
      speed_delay = 1000;
      tetris_gameover = 0;
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
    if (action & ACT_RP) {
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
          tetris_gameover = 1;
        }
      }
      last_tick = millis();
    }
    if (!tetris_gameover) {
      render();
    }
}

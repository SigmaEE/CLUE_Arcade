///// LED Related
#include <FastLED.h>
#include <EEPROM.h>
#define LED_PIN  3

#define COLOR_ORDER GRB
#define CHIPSET     WS2812B

#define BRIGHTNESS 64

// Params for width and height
const uint8_t kMatrixWidth = 30;
const uint8_t kMatrixHeight = 58;

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);



uint16_t XY( uint8_t x, uint8_t y)
{


  uint16_t i;

if( !( x<0 || x >= kMatrixWidth || y<0 || y >= kMatrixHeight )){

  if ( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if ( kMatrixSerpentineLayout == true) {
    if ( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }

  return i;
}

return 2000;
}

void clearScreen() {
  for ( int i = 0; i < kMatrixWidth; i++) {
    for (int j = 0; j < kMatrixHeight; j++) {
      leds[XY(i, j)] = CRGB::Black;
    }
  }
  //FastLED.show();
}


///////////////////Snake Game

int xPotVal;
int yPotVal;
const int xPotPin = A0;
const int yPotPin = A1;


bool gameOver;
const int w = kMatrixWidth;
const int h = kMatrixHeight;

int x, y,  fruitX, fruitY, score, highScore, count;
int speed_delay = 1000;

// 1,2,3,4 up,right,down,left
int dir = 2;

int tailX[600];
int tailY[600];
int nTail = 40;


uint8_t readHighScore() {
  static uint8_t tmpScore = 0;
  static uint8_t memHighScore = 0;
  
  for (int i=0; i<1024; i++) {
    tmpScore = EEPROM.read(i);
    if (tmpScore > memHighScore) {
      memHighScore = tmpScore;
    }
  }
  return memHighScore;
}

void writeHighScore() {
  static uint8_t tmpAdr = 0;
  while (EEPROM.read(tmpAdr) != 0) {
    tmpAdr++;
  }
  EEPROM.write(tmpAdr, highScore);
  
}

void inPut() {

  xPotVal = analogRead(xPotPin);
  yPotVal = analogRead(yPotPin);

  Serial.print("xval = ");

  Serial.println(xPotVal);

  Serial.print("yval = ");

  Serial.println(yPotVal);


  if ( xPotVal < 400) {
    //    if(dir = 4){
    //      gameOver = true;
    //    }
    dir = 4;
  } else if (xPotVal > 600) {
    //        if(dir = 2){
    //      gameOver = true;
    //    }
    dir = 2;
  }
  if (yPotVal < 400) {
    //        if(dir = 1){
    //      gameOver = true;
    //    }
    dir = 3;
  } else if (yPotVal > 600) {
    //            if(dir = 3){
    //      gameOver = true;
    //    }
    dir = 1;
  }
}

void deathAnimation() {
  Serial.println("pre animation");

  delay(100);
  for (int i = -1; i < 2; i++) {
    leds[XY(x + i, y + 1)] = CRGB::Red;
    leds[XY(x + i, y - 1)] = CRGB::Red;
  }
  leds[XY(x - 1, y)] = CRGB::Red;
  leds[XY(x + 1, y)] = CRGB::Red;
  FastLED.show();
  delay(100);


  for (int i = -2; i < 3; i++) {
    leds[XY(x + i, y + 2)] = CRGB::Red;
    leds[XY(x + i, y - 2)] = CRGB::Red;
  }
  for (int i = -1; i < 2; i++) {
    leds[XY(x - 2, y + i)] = CRGB::Red;
    leds[XY(x + 2, y + i)] = CRGB::Red;
  }

  FastLED.show();
  delay(100);


  for (int i = -3; i < 4; i++) {
    leds[XY(x + i, y + 3)] = CRGB::Red;
    leds[XY(x + i, y - 3)] = CRGB::Red;
  }
  for (int i = -2; i < 3; i++) {
    leds[XY(x - 3, y + i)] = CRGB::Red;
    leds[XY(x + 3, y + i)] = CRGB::Red;
  }

  FastLED.show();
  delay(100);



  Serial.println("animation");
}

void hsAnimation() {
  Serial.println("pre animation");

  delay(100);
  for (int y = 0; y < 57; ++y) {
    for (int x = 0; x < 29; ++x) {
      leds[XY(x,y)] = CRGB::Green;
      }
  }
  delay(100);



  Serial.println("animation");
}

void fruitAnimation() {
  Serial.println("pre animation");

  delay(100);
  for (int i = -1; i < 2; i++) {
    leds[XY(fruitX + i, fruitY + 1)] = CRGB::ForestGreen;
    leds[XY(fruitX + i, fruitY - 1)] = CRGB::ForestGreen;
  }
  leds[XY(fruitX - 1, fruitY)] = CRGB::ForestGreen;
  leds[XY(fruitX + 1, fruitY)] = CRGB::ForestGreen;
  FastLED.show();
  delay(100);


  for (int i = -2; i < 3; i++) {
    leds[XY(fruitX + i, fruitY + 2)] = CRGB::Green;
    leds[XY(fruitX + i, fruitY - 2)] = CRGB::Green;
  }
  for (int i = -1; i < 2; i++) {
    leds[XY(fruitX - 2, fruitY + i)] = CRGB::Green;
    leds[XY(fruitX + 2, fruitY + i)] = CRGB::Green;
  }

  FastLED.show();
  delay(100);


  for (int i = -3; i < 4; i++) {
    leds[XY(fruitX + i, fruitY + 3)] = CRGB::ForestGreen;
    leds[XY(fruitX + i, fruitY - 3)] = CRGB::ForestGreen;
  }
  for (int i = -2; i < 3; i++) {
    leds[XY(fruitX - 3, fruitY + i)] = CRGB::ForestGreen;
    leds[XY(fruitX + 3, fruitY + i)] = CRGB::ForestGreen;
  }

  FastLED.show();
  delay(100);



  Serial.println("animation");
}

void logic() {

  int prevX = tailX[0];
  int prevY = tailY[0];
  int prev2X, prev2Y;
  tailX[0] = x;
  tailY[0] = y;

  for ( int i =  1; i < nTail; i++) {
    prev2X = tailX[i];
    prev2Y = tailY[i];
    tailX[i] = prevX;
    tailY[i] = prevY;
    prevX = prev2X;
    prevY = prev2Y;
  }


  if (dir == 1) {
    if (y < h - 1) {
      y++;
    } else {
      y = 0;
    }

  } else if (dir == 3) {
    if (y > 0) {
      y--;
    } else {
      y = kMatrixHeight-1;
    }
  } else if (dir == 2) {
    if (x < w - 1) {
      x++;
    } else {
      x = 0;
    }
  }
  else if (dir == 4) {
    if (x > 0) {
      x--;
    } else {
      x = 29;
    }
  }


  for (int i = 0; i < nTail; i++) {
    if (tailX[i] == x && tailY[i] == y) {
      gameOver = true;
    }
  }


  if (x == fruitX && y == fruitY) {
    score++;
    nTail++;
    fruitAnimation();
    fruitX = rand() % w;
    fruitY = rand() % h;

  }
}

void draw() {

  clearScreen();

  for ( int i = 0; i < w; i++) {

    for (int j = 0; j < h; j++) {

      if ( i == x && j == y) {
        leds[XY(i, j)] = CRGB::Blue;

      } else if ( i == fruitX && j == fruitY) {
        leds[XY(i, j)] = CRGB::Green;

      } else {
        for (int k = 0; k < nTail; k++) {

          if (tailX[k] == i && tailY[k] == j) {
            leds[XY(i, j)] = CRGB::Purple;
          }
        }

        // leds[XY(tailX[nTail], tailY[nTail])] = CRGB::Black;

      }
    }
  }

  FastLED.show();

  //  Serial.print("xpos = ");
  //  Serial.println(x);
  //  Serial.print("ypos = ");
  //  Serial.println(y);


}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

//#define CONSOLE_INPUT
#ifdef CONSOLE_INPUT
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
    //        if(dir = 2){
    //      gameOver = true;
    //    }
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
    
    //            if(dir = 3){
    //      gameOver = true;
    //    }
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

long int last_tick;



void setup() {
  Serial1.begin(9600);
  Serial1.println("Setting up");
  gameOver = false;
  x = w / 2;
  y = h / 2;

  fruitX = rand() % w;
  fruitY = rand() % h;
  score = 0;
  


 
  highScore = readHighScore();
  Serial1.println(highScore);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  clearScreen();
  FastLED.show();

  // leds[XY(tailX[nTail], tailY[nTail])] = CRGB::Black;

// tetris stuff
  last_tick = millis();
  new_piece(&active_piece);
  render();

}

void snake_loop() {
  while (!gameOver) {
    //  Serial.println("Game is running");
    draw();
    inPut();
    logic();
    delay(5);
  }

  while (gameOver) {

    deathAnimation();

    for (int j = 0; j < kMatrixHeight; j++) {
      for ( int i = 0; i < kMatrixWidth; i++) {
        leds[XY(i, j)] = CRGB::Red;
      }
    }

    FastLED.show();
    delay(150);
    clearScreen();
    FastLED.show();
    delay(150);
    for (int j = 0; j < kMatrixHeight; j++) {
      for ( int i = 0; i < kMatrixWidth; i++) {
        leds[XY(i, j)] = CRGB::Red;
      }
    }

    FastLED.show();
    delay(150);
    clearScreen();
    FastLED.show();

    //
    //    for (int j = 0; j < kMatrixHeight; j++) {
    //      for ( int i = 0; i < kMatrixWidth; i++) {
    //
    //        leds[XY(i, j)] = CRGB::Black;
    //      }
    //      FastLED.show();
    //    }


    gameOver = false;
    nTail = 0;


  }
  //
  //FastLED.show();
  //delay(30);

}
int tetris_gameover = 0;
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

void loop() {
  // snake_loop();
  tetris_loop();
}

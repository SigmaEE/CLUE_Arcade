///// LED Related
#include <FastLED.h>
#include <EEPROM.h>
#define LED_PIN  3

#define COLOR_ORDER GRB
#define CHIPSET     WS2812B

int BRIGHTNESS = 50;
#define Letter_size_row 5
#define Letter_size_col 4
#define Arrow_size_row 5
#define Arrow_size_col 5

#define ACT_NONE 0
#define ACT_L  (1 << 0)
#define ACT_R  (1 << 1)
#define ACT_D  (1 << 2)
#define ACT_U (1 << 3)
#define ACT_RN (1 << 4)
#define ACT_Q  (1 << 5)

#define first_snake_score_adress 0
#define last_snake_score_adress 255
#define first_tetris_score_adress 256
#define last_tetris_score_adress 512

// Params for width and height
#define kMatrixWidth (30)
#define kMatrixHeight (58)

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;

int xPotVal;
int yPotVal;
const int xPotPin = A0;
const int yPotPin = A1;

const int w = kMatrixWidth;
const int h = kMatrixHeight;
const int start_h = 16;
const int end_h = h;

bool GameOver;
bool GameOver_tetris;
bool GameOver_snake;
bool selection;
int x, y,  highScore_snake, highScore_tetris;
int letter[Letter_size_row][Letter_size_col];
String choice  = "none";
void home_screen();
void game_selection();
int check_input();
void snake_setup();
void tetris_setup();
void snake_loop();
void tetris_loop();

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  if ( !( x < 0 || x >= kMatrixWidth || y < 0 || y >= kMatrixHeight )) {
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
      case '9': action |= ACT_U; break;
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

  //Serial.print("xval = ");
  //Serial.println(xPotVal);
  //Serial.print("yval = ");
  //Serial.println(yPotVal);

  current_time = millis();

  if ( xPotVal < 400) {
    if (current_time - last_left > 200) {
      action |= ACT_L;
      last_left = millis();
    }
  } else if (xPotVal > 600) {
    if (current_time - last_right > 200) {
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
      action |= ACT_U;
      last_rot = millis();
    }
  }
  return action;
  // compare current input to new input.
  // if new input then use it
  // if repeating input (holds left, right or down)
  // then dont use it until after some time has passed
}

void clearScreen() {
  for ( int i = 0; i < kMatrixWidth; i++) {
    for (int j = 0; j < kMatrixHeight; j++) {
      leds[XY(i, j)] = CRGB::Black;
    }
  }
  //FastLED.show();
}

char * dummy_sprite[] = {
  (char*)"5 5 5 1",
  (char*)". c #660000",
  (char*)"b c #56f9ff",
  (char*)"# c #92f4df",
  (char*)"a c #67efbf",
  (char*)"c c #15e781",
  (char*)".....",
  (char*)".....",
  (char*)"..#..",
  (char*)".aba.",
  (char*)"ca#ac"
};

// example usage: draw_xpm(dummy_sprite, 10, 10);
void draw_xpm(char * xpm[], int xofs, int yofs)
{
  int w, h, n_colors, depth;
  char dummy;
  sscanf(xpm[0], "%d %d %d %d", &w, &h, &n_colors, &depth);

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      // loop through and parse colors
      int r, g, b;
      for (char ** col_str = &xpm[1]; col_str <= &xpm[n_colors]; col_str++) {
        if (**col_str == xpm[n_colors + 1 + y][x]) { // match! grab color value
          sscanf(*col_str, "%*c %*c #%02x%02x%02x", &r, &g, &b);

        }
      }
      // printf("(%02x%02x%02x)\n", r, g, b);
      leds[XY(xofs + x, yofs + y)].r = r;
      leds[XY(xofs + x, yofs + y)].g = g;
      leds[XY(xofs + x, yofs + y)].b = b;
    }
  }
}

uint8_t readHighScore() {
  static uint8_t tmpScore = 0;
  static uint8_t memHighScore_snake = 0;
  static uint8_t memHighScore_tetris = 0;
  if (choice == "snake") {
    for (int i = first_snake_score_adress; i < last_snake_score_adress + 1; i++) {
      tmpScore = EEPROM.read(i);
      if (tmpScore > memHighScore_snake) {
        memHighScore_snake = tmpScore;
      }
    }
  }
  else if (choice == "tetris") {
    for (int i = first_tetris_score_adress; i < last_tetris_score_adress + 1; i++) {
      tmpScore = EEPROM.read(i);
      if (tmpScore > memHighScore_tetris) {
        memHighScore_tetris = tmpScore;
      }
    }
  }
  if (choice  == "snake") {
    Serial.println("snake highscore = ");
    Serial.println(memHighScore_snake);
    return memHighScore_snake;
  }
  else if (choice  == "tetris") {
    return memHighScore_tetris;
  }

}

void writeHighScore() {
  Serial.println("write highscore");
  Serial.print("Game = ");
  Serial.println(choice );
  int tmpAdr = 0;
  if (choice  == "snake") {
    tmpAdr = first_snake_score_adress;
    while (EEPROM.read(tmpAdr) != 0 && tmpAdr <= last_snake_score_adress) {
      tmpAdr++;
    }
    EEPROM.write(tmpAdr, highScore_snake);
  }
  else if (choice  == "tetris") {
    tmpAdr = first_tetris_score_adress;
    while (EEPROM.read(tmpAdr) != 0 && tmpAdr <= last_tetris_score_adress) {
      tmpAdr++;
      delay(100);
    }
    EEPROM.write(tmpAdr, highScore_tetris);
    
  }
}

void deathAnimation() {

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
}

void hsAnimation() {
  Serial.println("pre animation");

  delay(100);
  for (int y = 0; y < 57; ++y) {
    for (int x = 0; x < 29; ++x) {
      leds[XY(x, y)] = CRGB::Green;
    }
  }
  delay(100);

  Serial.println("animation");
}

void setup() {
  delay(1000);
  Serial1.begin(9600);
  Serial1.println("Setting up");

  x = w / 2;
  y = h / 2;
  choice  = "snake";
  highScore_snake = readHighScore();
  choice  = "tetris";
  highScore_tetris = readHighScore();
  choice  = "";


  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  GameOver = true;
  selection = false;
  //clearScreen();
  //home_screen();

}

// leds[XY(tailX[nTail], tailY[nTail])] = CRGB::Black;

//snake_setup();
//tetris_setup();
void tester();
void bright_setting();
void loop() {

  if (GameOver) {
    clearScreen();
    home_screen();
    game_selection();

  }


  //GameOver = false;
  if (choice  == "snake") {
    snake_loop();
  }
  else if (choice  == "tetris") {
    tetris_loop();
  }

  else if (choice  == "setting") {
    Serial.println("setting chosen");
    clearScreen();
    setting_screen();

    if (choice == "bright") {
      bright_setting();
    }
  }

}

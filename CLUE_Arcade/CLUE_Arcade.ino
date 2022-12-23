///// LED Related
#include <FastLED.h>
#include <EEPROM.h>
#include "src/CLUE_Arcade.h"
#include "src/Mem_high_score.h"
#include "src/joystick_input.h"

#define LED_PIN  3

// fastled takes 53/52 milliseconds to render!
#define COLOR_ORDER GRB
#define CHIPSET WS2812B

int BRIGHTNESS = 50;
#define Letter_size_row 5
#define Letter_size_col 4
#define Arrow_size_row 5
#define Arrow_size_col 5

#define ACT_NONE 0
#define ACT_L_P1  (1 << 0)
#define ACT_R_P1  (1 << 1)
#define ACT_D_P1  (1 << 2)
#define ACT_U_P1  (1 << 3)
#define ACT_B1_P1 (1 << 4)
#define ACT_B2_P1 (1 << 5)
#define ACT_RN_P1 (1 << 6)
#define ACT_Q_P1  (1 << 7)

#define ACT_L_P2  (1 << 8)
#define ACT_R_P2  (1 << 9)
#define ACT_D_P2  (1 << 10)
#define ACT_U_P2  (1 << 11)
#define ACT_B1_P2 (1 << 12)
#define ACT_B2_P2 (1 << 13)
#define ACT_RN_P2 (1 << 14)
#define ACT_Q_P2  (1 << 15)

// OLD
#define ACT_L  (1 << 0)
#define ACT_R  (1 << 1)
#define ACT_D  (1 << 2)
#define ACT_U  (1 << 3)
#define ACT_RN (1 << 6)
#define ACT_Q  (1 << 7)

int up_delay = 200;
int right_delay = 200;
int down_delay = 200;
int left_delay = 200;
int b1_delay = 200;
int b2_delay = 200;


// Params for width and height
#define kMatrixWidth (30)
#define kMatrixHeight (58)

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;

#define player1Up 8
#define player1Right 10
#define player1Left 9
#define player1Down 11
#define player1B1 6
#define player1B2 7

#define player2Up 15
#define player2Right 14
#define player2Left 18
#define player2Down 17
#define player2B1 16
#define player2B2 19

#define RANDOM_PIN A6

const int w = kMatrixWidth;
const int h = kMatrixHeight;
const int start_h = 16;
const int end_h = h;

int active_player = 0;
int number_of_players = 0;
bool GameOver;
bool selection;
int x, y;
int letter[Letter_size_row][Letter_size_col];
String choice  = "none";
void home_screen_loop();
void home_screen_setup();
void input_screen_loop();
void input_screen_setup();
void game_selection();

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);

struct hs_board curr_tetris_board;
struct hs_board curr_snake_board;

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

struct input_filter {
  int up_initial_delay;
  int up_repeat_delay;
  int down_initial_delay;
  int down_repeat_delay;
  int left_initial_delay;
  int left_repeat_delay;
  int right_initial_delay;
  int right_repeat_delay;
};

int get_filtered_input(struct input_filter * filter)
{

  return 0;
}

void clearScreen() {
  for ( int i = 0; i < kMatrixWidth; i++) {
    for (int j = 0; j < kMatrixHeight; j++) {
      leds[XY(i, j)] = CRGB::Black;
    }
  }
}

// example usage: draw_xpm(dummy_sprite, 10, 10);
void draw_xpm(char * xpm[], int xofs, int yofs)
{
  int w, h, n_colors, depth;
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
      leds[XY(xofs - x, yofs + y)].r = r;
      leds[XY(xofs - x, yofs + y)].g = g;
      leds[XY(xofs - x, yofs + y)].b = b;
    }
  }
}

void draw_color_xpm(int color, char * xpm[], int xofs, int yofs)
{
  int w, h, n_colors, depth;
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
      if (r != 0x00 && g != 0x00 && b != 0x00) {
        leds[XY(xofs - x, yofs + y)] = color;
      } else {
        leds[XY(xofs - x, yofs + y)].r = r;
        leds[XY(xofs - x, yofs + y)].g = g;
        leds[XY(xofs - x, yofs + y)].b = b;
      }
    }
  }
}

void deathAnimation()
{
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

void tester();
void bright_setting();


struct arcade_screen home_screen = {
  .name = (char*)"HOME",
  .loop_fn = &home_screen_loop,
  .setup_fn = &home_screen_setup
};

struct arcade_screen letter_input_screen = {
  .name = (char*)"INPUT",
  .loop_fn = &input_screen_loop,
  .setup_fn = &input_screen_setup
};

struct arcade_screen setting_screen = {
  .name = (char*)"CONFIG",
  .loop_fn = &setting_screen_loop,
  .setup_fn = &setting_screen_setup
};

struct arcade_screen snake_options_screen = {
  .name = (char*)"SNAKE OPTIONS",
  .loop_fn = &snake_options_loop,
  .setup_fn = &snake_options_setup
};

struct arcade_screen snake_multi_screen = {
  .name = (char*)"CONFIG",
  .loop_fn = &snake_multi_loop,
  .setup_fn = &snake_multi_setup
};



struct arcade_screen current_screen;
struct arcade_screen previous_screen;
int screen_transition = 0;

void switch_screen(struct arcade_screen * screen)
{
  // push/pop screen stack ??

  // setup screen transition
  //previous_screen = save_screen_image();
  //screen_transition_start = millis();

  current_screen = *screen;
  current_screen.setup_fn();
}

extern char ** numbers[];
void hiscore_screen_loop(void)
{

  int action;
  clearScreen();
  leds[0] = 0xFFFFFF;
  int green = 0x0caf00;
  for (int i = 0; i < 9; i++) {
    hs_player p = curr_tetris_board.scores[i];
    int tmp_hs = p.score;
    p.name[3] = '\0';
    draw_color_xpm(green, numbers[tmp_hs % 10],3,i * 6);
    draw_color_xpm(green, numbers[(tmp_hs/10) % 10],7,i * 6);
    draw_color_xpm(green, numbers[(tmp_hs/100) % 10],11,i * 6);
    draw_color_xpm(green, numbers[(tmp_hs/1000) % 10],15,i * 6);
    //draw_color_xpm(green, numbers[(tmp_hs/10000) % 10],19,0);
    draw_word(29, i * 6, p.name, 0xff7300);
  }
  FastLED.show();

  action = check_joystick_input();

  if (action & ACT_U_P1 || action & ACT_D_P1 || action & ACT_U_P2 || action & ACT_D_P2) {
	  switch_screen(&home_screen);
  }
  delay(100);
}

void hiscore_screen_setup(void)
{
}

struct arcade_screen hiscore_screen = {
  .name = (char*)"HISCOR",
  .loop_fn = &hiscore_screen_loop,
  .setup_fn = &hiscore_screen_setup
};

void screen_transition_loop(void)
{
  if (screen_transition) {
    // do screen transition stuff!
    // use combination of saved screen graphics and
    // current leds and show them neatly!
    // set screen_transition to 0 when done!
  }

}

int letter_input_done = 0;
char letter_input_buffer[10] = "";

void new_hs_player(int score, struct hs_player &player)
{
  letter_input_done = 0;
  input_screen_setup();
  while (letter_input_done == 0) {
    input_screen_loop();
  }
  player.score = score;
  //input prompt
  player.name[0] = letter_input_buffer[0];
  player.name[1] = letter_input_buffer[1];
  player.name[2] = letter_input_buffer[2];
  player.name[3] = '\0';
}

void setup()
{
  delay(1000);
  Serial1.begin(9600);
  Serial1.println("Setting up");

  pinMode(player1B1, INPUT_PULLDOWN);
  pinMode(player1B2, INPUT_PULLDOWN);
  pinMode(player1Up, INPUT_PULLUP);
  pinMode(player1Right, INPUT_PULLUP);
  pinMode(player1Left, INPUT_PULLUP);
  pinMode(player1Down, INPUT_PULLUP);

  pinMode(player2B1, INPUT_PULLDOWN);
  pinMode(player2B2, INPUT_PULLDOWN);
  pinMode(player2Up, INPUT_PULLUP);
  pinMode(player2Right, INPUT_PULLUP);
  pinMode(player2Left, INPUT_PULLUP);
  pinMode(player2Down, INPUT_PULLUP);

  pinMode(RANDOM_PIN, INPUT);

  x = w / 2;
  y = h / 2;
  // Update score board from EEPROM
  read_high_score_board("t", curr_tetris_board);
  read_high_score_board("s", curr_snake_board);
  choice  = "";

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  GameOver = true;
  selection = false;
  current_screen = home_screen;
}

void loop()
{
  current_screen.loop_fn();
  if (choice == "bright") {
    bright_setting();
  }
}

#include "src/tetris.h"
#include "src/snake.h"
#include "src/joystick_input.h"

int scroll_move_home = 0;
int menu_pos = 0;
int active_player = 0;


int clue_length = 4;
int clue_start_pos_col = -1 + (kMatrixWidth/2 - 1) + ((Letter_size_col + 1) * (clue_length - 1) + Letter_size_col)/2;
int clue_start_pos_row = 0;

int arcade_start_pos_col = -1 + (kMatrixWidth - 1);
int arcade_start_pos_row = clue_start_pos_row + (Letter_size_row - 1) + line_margin;

int snake_start_pos_col = (kMatrixWidth - 1);
int snake_start_pos_row = arcade_start_pos_row + (Letter_size_row - 1) + 8;

int line1_start_pos_row = snake_start_pos_row - line_margin;
int line1_start_pos_col = (kMatrixWidth - 1 - line_intake);
int line1_length = (kMatrixWidth - line_intake * 2);

int line2_start_pos_row = snake_start_pos_row + (Letter_size_row - 1) + line_margin;
int line2_start_pos_col = (kMatrixWidth - 1 - line_intake);
int line2_length = (kMatrixWidth - line_intake * 2);

int arrow_down_start_pos_col = kMatrixWidth / 2 + Arrow_size_col / 2;
int arrow_down_start_pos_row = kMatrixHeight - Arrow_size_row;

int arrow_up_start_pos_col = kMatrixWidth / 2 + Arrow_size_col / 2;
int arrow_up_start_pos_row = arrow_down_start_pos_row - (Letter_size_row - 1) - 1;


void draw_letter(int x, int y, char let, uint32_t color)
{
  if ('q' == let ) let = 'A';
  if (let < 'A' || let > '\\') let = '\\';
  int letter[Letter_size_row][Letter_size_col];
  memcpy(letter, All_letters[let - 'A'], Letter_size_row * Letter_size_col * sizeof(int));
  for (int col = 0; col < Letter_size_col; col++) {
    for (int row = 0; row < Letter_size_row; row++) {
      if (letter[row][col] == 1) {
        leds[XY(x - col, y + row)] = color;
      }
    }
  }
}

void draw_word(int x, int y, char * str, uint32_t color)
{
  int i = 0;
  while ((str[i]) != '\0') {
    draw_letter(x - Letter_size_row * i, y, str[i], color);
    i++;
  }
}

void arrow_up_sign(int arrow_start_pos_col, int arrow_start_pos_row) {
  for (int col = 0; col < Arrow_size_col; col++) {
    for (int row = 0; row < Arrow_size_row; row++) {
      if (Arrow_up[row][col] == 1) {
        leds[XY(arrow_start_pos_col - col, arrow_start_pos_row + row)] = CRGB::White;
      }
    }
  }
}

void arrow_down_sign(int arrow_start_pos_col, int arrow_start_pos_row) {
  for (int col = 0; col < Arrow_size_col; col++) {
    for (int row = 0; row < Arrow_size_row; row++) {
      if (Arrow_down[row][col] == 1) {
        leds[XY(arrow_start_pos_col - col, arrow_start_pos_row + row)] = CRGB::White;
      }
    }
  }
}

void select_line(int line_start_pos_col, int line_length, int line_start_pos_row) {
  for (int col = 0; col < (line_length); col++) {
    leds[XY(line_start_pos_col - col, line_start_pos_row)] = CRGB::White;
  }
}

#define RANGE(from, to) ( (c >= from && c < to) * 1 )
long plasma_grad(double c)
{
	int r, g, b;
        r = g = b = 1;
        if (RANGE(0.3, 0.4)) {
		r = g = b = 10;
	}

	return b + (g << 8) + (r << 16);
}

void draw_plasma()
{
	int ofs = millis();
	ofs = ofs * 0.06;
	for (int x = 0; x < 30; x++) {
		for (int y = 0; y < 60; y++) {
			double dx = 15.0 - x;
			double dy = 30.0 - y;
			dx = 15 - x * cos(ofs * 0.03) * 0.2;
			dy = 15 - y * sin(ofs * 0.04) * 0.2;
			double d = sqrt(dx*dx + dy*dy);
			double base = 2 * (1.0 + sin(d * 0.2 - (double)ofs * 0.01));

			dx = 15 - x * cos(ofs * 0.02);
			dy = 15 - y * sin(ofs * 0.01);
			double d1 = sqrt(0.8 * dx * dx + 1.3 * dy * dy) * 0.2;
			double d2 = sqrt(1.35 * dx * dx + 0.45 * dy  * dy) * 0.2;

			double h = 1.0 + sin(d1 + ofs * 0.1) + 1.0 + sin(d2 + ofs * 0.05);
			int color = base * 10 + h * 40.0;
			color = plasma_grad((base + h) / 8);
			leds[XY(x, y)] = color;
		}
	}
}

void home_screen_setup()
{
  choice = "";
}

struct menu_item {
  char * text;
  uint32_t color;
  struct arcade_screen * screen_1;
  struct arcade_screen * screen_2;
};

struct menu_item home_menu[] = {
  { (char*)"SNAKE", CRGB::Yellow, &snake_screen, &hiscore_screen },
  { (char*)"TETRIS", CRGB::Red, &tetris_screen, &hiscore_screen },
  { (char*)"CONFIG", CRGB::Green, &setting_screen, &home_screen },
  //{ (char*)"TEST", CRGB::Purple, &letter_input_screen, &home_screen },
  { (char*)"HISCORE", CRGB::Cyan, &hiscore_screen, &hiscore_screen }
};

void draw_menu(struct menu_item * menu, int nitems, int x, int y)
{
  CRGB col;
  col = CRGB::Yellow;

  for (int i = 0; i < nitems; ++i) {
    draw_word(x, y + 8 * i, menu[i].text, menu[i].color);
  }
}

void home_screen_loop()
{
  int action = ACT_NONE;
  GameOver = true;
  choice  = "";

  clearScreen();

  draw_word(clue_start_pos_col + 1, clue_start_pos_row + 1, (char*)"CLUE", 0x000020);
  draw_word(arcade_start_pos_col + 1, arcade_start_pos_row  + 1+ 1, (char*)"ARCADE", 0x000020);
  draw_word(clue_start_pos_col, clue_start_pos_row, (char*)"CLUE", 0x0000ff);
  draw_word(arcade_start_pos_col, arcade_start_pos_row + 1, (char*)"ARCADE", 0x0000ff);

  select_line(line1_start_pos_col, line1_length, menu_pos * 8 + line1_start_pos_row);
  select_line(line2_start_pos_col, line2_length, menu_pos * 8 + line2_start_pos_row);

  draw_menu(home_menu, sizeof(home_menu) / sizeof(home_menu[0]), snake_start_pos_col, snake_start_pos_row);

  arrow_up_sign(arrow_up_start_pos_col, arrow_up_start_pos_row);
  arrow_down_sign(arrow_down_start_pos_col, arrow_down_start_pos_row);

  FastLED.show();
  if (selection == false) {
    action = check_joystick_input();
    delay(1);
    if ((action & ACT_U_P1) || (action & ACT_U_P2)) {
      if (menu_pos > 0) {
        menu_pos--;
        scroll_move_home -= (Letter_size_row + word_margin - 1);
      }
    }
    if ((action & ACT_D_P1) || (action & ACT_D_P2))  {
      if (menu_pos < (sizeof(home_menu) / sizeof(home_menu[0])) - 1) {
        menu_pos++;
        scroll_move_home += (Letter_size_row + word_margin - 1);
      }
    }
    if ((action & ACT_R_P1) || (action & ACT_R_P2)) {
      selection = true;
      delay(1);
      if (action & ACT_R_P1) active_player = 1;
      else active_player = 2;
      switch_screen(home_menu[menu_pos].screen_1);
    }
    if ((action & ACT_L_P1) || (action & ACT_L_P2)) {
      selection = true;
      delay(1);
      switch_screen(home_menu[menu_pos].screen_2);
    }
    Serial.println(action);
    selection = false;
    GameOver = false;
  }
}

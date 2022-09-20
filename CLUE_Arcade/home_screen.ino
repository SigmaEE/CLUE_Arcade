//int line_margin = 2;
//int word_margin = 4;

int scroll_move_home = 0;

int clue_length = 4;
int clue_start_pos_col = (kMatrixWidth/2 - 1) + ((Letter_size_col + 1) * (clue_length - 1) + Letter_size_col)/2;
int clue_start_pos_row = 0;

int arcade_length = 6;
int arcade_start_pos_col = (kMatrixWidth - 1);
int arcade_start_pos_row = clue_start_pos_row + (Letter_size_row - 1) + line_margin;

int snake_length = 5;
int snake_intake = 0;
int snake_start_pos_col = (kMatrixWidth - 1) - snake_intake;
int snake_start_pos_row = arcade_start_pos_row + (Letter_size_row - 1) + 8;

int line1_start_pos_row = snake_start_pos_row - line_margin;
//int line_intake = 0;
int line1_start_pos_col = (kMatrixWidth - 1 - line_intake);
int line1_length = (kMatrixWidth - line_intake * 2);

int line2_start_pos_row = snake_start_pos_row + (Letter_size_row - 1) + line_margin;
int line2_start_pos_col = (kMatrixWidth - 1 - line_intake);
int line2_length = (kMatrixWidth - line_intake * 2);

int tetris_length = 6;
int tetris_start_pos_col = (kMatrixWidth - 1);
int tetris_start_pos_row = snake_start_pos_row + (Letter_size_row - 1) + word_margin;

//int seting_length = 6;
int seting_home_start_pos_col = (kMatrixWidth - 1);
int seting_home_start_pos_row = tetris_start_pos_row + (Letter_size_row - 1) + word_margin;

int arrow_down_start_pos_col = kMatrixWidth / 2 + Arrow_size_col / 2;
int arrow_down_start_pos_row = kMatrixHeight - Arrow_size_row;

int arrow_up_start_pos_col = kMatrixWidth / 2 + Arrow_size_col / 2;
int arrow_up_start_pos_row = arrow_down_start_pos_row - (Letter_size_row - 1) - 1;

int arrow_right_start_pos_col = snake_start_pos_col - snake_length * (Letter_size_col + 1);
int arrow_right_start_pos_row = snake_start_pos_row;

void draw_letter(int x, int y, char let, CRGB::HTMLColorCode color)
{
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

void draw_word(int x, int y, char * str, CRGB::HTMLColorCode color)
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

void arrow_right_sign(int arrow_start_pos_col, int arrow_start_pos_row, int scroll_move_home) {
  for (int col = 0; col < Arrow_size_col; col++) {
    for (int row = 0; row < Arrow_size_row; row++) {
      if (Arrow_right[row][col] == 1) {
        leds[XY(arrow_start_pos_col - col, arrow_start_pos_row + row + scroll_move_home)] = CRGB::White;
      }
    }
  }
}

void select_line(int line_start_pos_col, int line_length, int line_start_pos_row, int scroll_move_home) {
  for (int col = 0; col < (line_length); col++) {
    leds[XY(line_start_pos_col - col, line_start_pos_row + scroll_move_home)] = CRGB::White;
  }
}

void game_selection();

#define RANGE(from, to) ( (c >= from && c < to) * (c - from) )
long plasma_grad(double c)
{
	int r, g, b;

	b = 55 + RANGE(0.0, 1.0) * 200;
	r = RANGE(0.4, 1.0) * 200;
	g = RANGE(0.8, 1.0) * 255;

	b = b * 0.5;
	r = r * 0.5;
	g = g * 0.5;
	//r = (c > 0.4 && c < 0.43) * ((c - 0.4) / 0.03) * 255;
	//g = (c > 0.6 && c < 0.63) * 255;
	//b = (c > 0.8 && c < 0.83) * 255;
	//b = g = 0;
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
			//put_pixel(im, psize, x, y, color);
		}
	}
}

void home_screen()
{
  GameOver = true;
  choice  = "";

  draw_word(clue_start_pos_col, clue_start_pos_row, (char*)"CLUE", CRGB::Blue);
  draw_word(arcade_start_pos_col, arcade_start_pos_row, (char*)"ARCADE", CRGB::Blue);
  select_line(line1_start_pos_col, line1_length, line1_start_pos_row, scroll_move_home);
  draw_word(snake_start_pos_col, snake_start_pos_row, (char*)"SNAKE", CRGB::Yellow);
  select_line(line2_start_pos_col, line2_length, line2_start_pos_row, scroll_move_home);
  draw_word(tetris_start_pos_col, tetris_start_pos_row, (char*)"TETRIS", CRGB::Red);
  draw_word(seting_home_start_pos_col, seting_home_start_pos_row, (char*)"SETING", CRGB::Green);
  arrow_up_sign(arrow_up_start_pos_col, arrow_up_start_pos_row);
  arrow_down_sign(arrow_down_start_pos_col, arrow_down_start_pos_row);

  FastLED.show();
}

void game_selection() {
  int action = check_input();
  if (selection == false) {
    action = check_input();
    while (action == ACT_NONE || action == ACT_L) { //home screen stays until up or down input on joystick
      action = check_input();
      delay(1);
      if (action & ACT_U) {
        if (scroll_move_home > 0) {
          scroll_move_home -= (Letter_size_row + word_margin - 1);
          Serial.println(action);
          Serial.println("up");
        }
      }
      if (action & ACT_D) {
        if (scroll_move_home < 2 * (Letter_size_row + word_margin - 1)) {
          scroll_move_home += (Letter_size_row + word_margin - 1);
          Serial.println("down");
          Serial.println(action);
        }
      }

      if (action & ACT_R) {
        Serial.println("right");
        Serial.println("action is: ");
        Serial.println(action);
        Serial.println(selection);
        selection = true;
        Serial.println(selection);
        Serial.println(scroll_move_home);
        delay(1);
      }
    }

  } else {
    Serial.println("Exited while loop");
    if (scroll_move_home == 0 * (Letter_size_row + word_margin - 1)) {
      choice  = "snake";
    } else if (scroll_move_home == 1 * (Letter_size_row + word_margin - 1)) {
      choice  = "tetris";
    } else {
      choice  = "setting";
    }
    Serial.println("game selected");
    Serial.println(choice );
    if (choice  == "snake") {
      snake_setup();
    } else if (choice  == "tetris") {
      tetris_setup();
    }
    Serial.println("setted up");
    selection = false;
    GameOver = false;
  }
}

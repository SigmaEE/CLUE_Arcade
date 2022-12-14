int letter_cur_char = 0;
#define INPUT_MAX_LETTERS 3
char letter_input[INPUT_MAX_LETTERS + 1];
void draw_word(int x, int y, char * str, uint32_t color);
int input_start_time = 0;
void input_screen_setup()
{
   letter_cur_char = 0;
   for (int i = 0; i < INPUT_MAX_LETTERS; i++) {
     letter_input[i] = 'A';
   }
   letter_input[INPUT_MAX_LETTERS] = '\0';
   input_start_time = millis();
}

int get_cursor_color(void)
{
  #define PERIOD (2000)
  int tmp = (millis() - input_start_time) % PERIOD;
  double d = (double)tmp / (double)PERIOD;

  int color = sin(d * 3.1415926) * 255;
  return color + (color << 8) + (color << 16);
}

void input_screen_loop()
{
  int action = check_joystick_input();
  
  clearScreen();
  for (int y = 25; y < 35; y++) {
    for (int x = 0; x < 25; x++)  {
      leds[XY(25 - x, y)] = 0x000000;
    }
  }

  draw_word(25, 25, letter_input, 0x804000);
  for (int x = 0; x < 4; x++)  {
    leds[XY(25 - 5 * letter_cur_char - x, 31)] = get_cursor_color();
  }

  draw_word(25 - 5 * (INPUT_MAX_LETTERS + 1), 25, (char*)"\\", 0x00ff00);
  FastLED.show();

  if (action & ACT_D_P1 || action & ACT_D_P2) {
    if (letter_cur_char < INPUT_MAX_LETTERS) {
      letter_input[letter_cur_char]++;
      if (letter_input[letter_cur_char] > 'V') {
        letter_input[letter_cur_char] = 'A';
      }
    }
  }
  if (action & ACT_U_P1 || action & ACT_U_P2 ) {
    if (letter_cur_char < INPUT_MAX_LETTERS) {
      letter_input[letter_cur_char]--;
      if (letter_input[letter_cur_char] < 'A') {
        letter_input[letter_cur_char] = 'V';
      }
    } else if (letter_cur_char == INPUT_MAX_LETTERS + 1) {
      // DONE!
      letter_input_done = 1;
      letter_input_buffer[0] = letter_input[0];
      letter_input_buffer[1] = letter_input[1];
      letter_input_buffer[2] = letter_input[2];
      letter_input_buffer[3] = '\0';

      switch_screen(&home_screen);
    }
  }
  if (action & ACT_L_P1 || action & ACT_L_P2) {
    letter_cur_char--;
    if (letter_cur_char < 0) {
      letter_cur_char = 0;
    }
  }
  if (action & ACT_R_P1 || action & ACT_R_P2) {
    letter_cur_char++;
    if (letter_cur_char > (INPUT_MAX_LETTERS + 1)) {
      letter_cur_char = 0;
    }
  }
}

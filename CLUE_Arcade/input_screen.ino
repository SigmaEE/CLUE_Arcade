int letter_cur_char = 0;
#define INPUT_MAX_LETTERS 3
char letter_input[INPUT_MAX_LETTERS + 1];
void draw_word(int x, int y, char * str, uint32_t color);
void input_screen_setup()
{
   letter_cur_char = 0;
   for (int i = 0; i < INPUT_MAX_LETTERS; i++) {
     letter_input[i] = 'A';
   }
   letter_input[INPUT_MAX_LETTERS] = '\0';
}

void input_screen_loop()
{

  for (int y = 25; y < 35; y++) {
    for (int x = 0; x < 25; x++)  {
      leds[XY(25 - x, y)] = 0x000000;
    }
  }

  draw_word(25, 25, letter_input, 0x804000);
  for (int x = 0; x < 4; x++)  {
    leds[XY(25 - 5 * letter_cur_char - x, 31)] = 0xe0e0e0;
  }

  draw_word(25 - 5 * (INPUT_MAX_LETTERS + 1), 25, (char*)"\\", 0x00ff00);
  FastLED.show();

  int action = check_input();
  if (action & ACT_U) {
    if (letter_cur_char == INPUT_MAX_LETTERS + 1) {
      switch_screen(&home_screen);
    } else if (letter_cur_char < INPUT_MAX_LETTERS) {
      letter_input[letter_cur_char]++;
      if (letter_input[letter_cur_char] > 'V') {
        letter_input[letter_cur_char] = 'A';
      }
    }
  }
  if (action & ACT_D) {
    if (letter_cur_char < INPUT_MAX_LETTERS) {
      letter_input[letter_cur_char]--;
      if (letter_input[letter_cur_char] < 'A') {
        letter_input[letter_cur_char] = 'V';
      }
    }
  }
  if (action & ACT_L) {
    letter_cur_char--;
    if (letter_cur_char < 0) {
      letter_cur_char = 0;
    }
  }
  if (action & ACT_R) {
    letter_cur_char++;
    if (letter_cur_char > (INPUT_MAX_LETTERS + 1)) {
      letter_cur_char = 0;
    }
  }
}

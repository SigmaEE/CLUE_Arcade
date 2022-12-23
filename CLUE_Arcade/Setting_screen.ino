#include <FastLED.h>

int line_margin = 2;
int word_margin = 4;
int scroll_move_setting = 0;

int seting_length = 6;
int seting_start_pos_row = 0;
int seting_start_pos_col = kMatrixWidth - 1;

int bright_length = 6;
int bright_start_pos_row = seting_start_pos_row + (Letter_size_row - 1) + word_margin;
int bright_start_pos_col = kMatrixWidth - 1;

int line1_setting_start_pos_row = bright_start_pos_row - line_margin;
int line_intake = 0;
int line1_setting_start_pos_col = (kMatrixWidth - 1 - line_intake);
int line1_setting_length = (kMatrixWidth - line_intake * 2);

int line2_setting_start_pos_row = bright_start_pos_row + (Letter_size_row - 1) + line_margin;
int line2_setting_start_pos_col = (kMatrixWidth - 1 - line_intake);
int line2_setting_length = (kMatrixWidth - line_intake * 2);

void select_line(int line_start_pos_col, int line_length, int line_start_pos_row);

void setting_screen_setup()
{
  choice = "setting";
}

void setting_selection();
void draw_word(int x, int y, char * str, uint32_t);

void setting_screen_loop()
{
  clearScreen();
  GameOver = true;
  choice = "";
  draw_word(seting_start_pos_col, seting_start_pos_row, (char*)"CONFIG", (uint32_t) CRGB::Green);
  select_line(line1_setting_start_pos_col, line1_setting_length, line1_setting_start_pos_row + scroll_move_setting);
  draw_word(bright_start_pos_col, bright_start_pos_row, (char*)"BRIGHT", (uint32_t) CRGB::White);
  select_line(line2_setting_start_pos_col, line2_setting_length, line2_setting_start_pos_row + scroll_move_setting);

  FastLED.show();
  setting_selection();
}

void setting_selection() {
  int action;
  action = check_joystick_input();
  if (setting_selection == false) {
    action = check_joystick_input();

    while (action == ACT_NONE) { //home screen stays until up or down input on joystick
      action = check_joystick_input();
      delay(1);
    }
    if (action & ACT_U_P1 || action & ACT_U_P2) {
      if (scroll_move_setting > 0) {
        scroll_move_setting -= (Letter_size_row + word_margin - 1);
      }
    }
    if (action & ACT_D_P1 || action & ACT_D_P2) {
      if (scroll_move_setting < 1 * (Letter_size_row + word_margin - 1)) {
        scroll_move_setting += (Letter_size_row + word_margin - 1);
      }
    }
    if (action & ACT_R_P1 || action & ACT_R_P2 ) {
      selection = true;
      delay(1);
    }
  } else {
    if (scroll_move_setting == 0 * (Letter_size_row + word_margin - 1)) {
      choice = "bright";
    }
    if (choice == "bright") {
      bright_setting();
    }
    selection = false;
    GameOver = false;
  }
}

void bright_setting() {
  int action;
  while (1) { //home screen stays until up or down input on joystick
    action = check_joystick_input();
    if (action & ACT_R_P1 || action & ACT_R_P2){
      BRIGHTNESS += 10;
      FastLED.setBrightness( BRIGHTNESS );
      FastLED.show();
    }
    else if (action & ACT_L_P1 || action & ACT_L_P2){
      BRIGHTNESS -= 10;
      FastLED.setBrightness( BRIGHTNESS );
      FastLED.show();
    }
    else if(action & ACT_U_P1 || action &  ACT_U_P2){
      GameOver = true;
      switch_screen(&home_screen);
      break;
    }
    delay(1);
  }
  GameOver = true;
}

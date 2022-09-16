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

void select_line(int line_start_pos_col, int line_length, int line_start_pos_row, int scroll_move = 0);
void seting_word(int start_pos_row, int start_pos_col);

void bright_word(int start_pos_row, int start_pos_col) {
  for (int current_letter = 0; current_letter < bright_length; current_letter++) { //itterating through each letter in the word we want to write.
    switch (current_letter) {
      case 0: memcpy(letter, B_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 1: memcpy(letter, R_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 2: memcpy(letter, I_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 3: memcpy(letter, G_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 4: memcpy(letter, H_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 5: memcpy(letter, T_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
    }

    for (int col = 0; col < Letter_size_col; col++) {

      for (int row = 0; row < Letter_size_row; row++) {
        if (letter[row][col] == 1) {
          leds[XY(start_pos_col - (Letter_size_col + 1) * current_letter - col, start_pos_row + row)] = CRGB::White;
        }
      }
    }
  }
}
void setting_selection();
int action;
void setting_screen()
{
  //delay(1000);
  clearScreen();
  GameOver = true;
  Serial.println("line start setting");
  Serial.println(line1_setting_start_pos_row);
  choice = "";
  seting_word(seting_start_pos_row, seting_start_pos_col);
  select_line(line1_setting_start_pos_col, line1_setting_length, line1_setting_start_pos_row, scroll_move_setting);
  Serial.print("line drawing");
  Serial.println(line1_setting_start_pos_col);
  Serial.println(line1_setting_length);
  Serial.println(line1_setting_start_pos_row);
  Serial.println(scroll_move_setting);
  bright_word(bright_start_pos_row, bright_start_pos_col);
  select_line(line2_setting_start_pos_col, line2_setting_length, line2_setting_start_pos_row, scroll_move_setting);

  FastLED.show();
  setting_selection();
}

void setting_selection() {
  action = check_input();
  if (setting_selection == false) {
    action = check_input();

    while (action == ACT_NONE) { //home screen stays until up or down input on joystick
      action = check_input();
      delay(1);
    }
    if (action & ACT_U) {
      if (scroll_move_setting > 0) {
        scroll_move_setting -= (Letter_size_row + word_margin - 1);

        Serial.println(action);
        Serial.println("up");
      }
    }
    if (action & ACT_D) {
      if (scroll_move_setting < 1 * (Letter_size_row + word_margin - 1)) {
        scroll_move_setting += (Letter_size_row + word_margin - 1);

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
      Serial.println(scroll_move_setting);
      delay(1);
    }
  } else {
    Serial.println("Exited while loop");
    if (scroll_move_setting == 0 * (Letter_size_row + word_margin - 1)) {
      choice = "bright";
    }
    Serial.println("choice selected");
    Serial.println(choice);
    if (choice == "bright") {
      bright_setting();
    }
    Serial.println("setted up");
    selection = false;
    GameOver = false;
  }
}

void bright_setting() {
  Serial.print("Bright setting");
  while (action == ACT_NONE || action == ACT_U || action == ACT_D) { //home screen stays until up or down input on joystick
    action = check_input();
    delay(1);
  }

  if (action & ACT_R){
    BRIGHTNESS += 10;
    Serial.print("brightness +");
    Serial.print(BRIGHTNESS);
  } else if (action & ACT_L){
    BRIGHTNESS -= 10;
    Serial.print("brightness -");
    Serial.print(BRIGHTNESS);
  }
}
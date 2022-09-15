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

void clue_word(int clue_start_pos_col, int clue_start_pos_row) {
  for (int current_letter = 0; current_letter < clue_length; current_letter++) { //itterating through each letter in the word we want to write.
    switch (current_letter) {
      case 0: memcpy(letter, C_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 1: memcpy(letter, L_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 2: memcpy(letter, U_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 3: memcpy(letter, E_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
    }

    for (int col = 0; col < Letter_size_col; col++) {
      for (int row = 0; row < Letter_size_row; row++) {
        if (letter[row][col] == 1) {
          leds[XY(clue_start_pos_col - (Letter_size_col + 1) * current_letter - col, clue_start_pos_row + row)] = CRGB::Blue;
        }
      }
    }
  }
}

void arcade_word(int arcade_start_pos_col, int arcade_start_pos_row) {
  for (int current_letter = 0; current_letter < arcade_length; current_letter++) { //itterating through each letter in the word we want to write.
    switch (current_letter) {
      case 0: memcpy(letter, A_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 1: memcpy(letter, R_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 2: memcpy(letter, C_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 3: memcpy(letter, A_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 4: memcpy(letter, D_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 5: memcpy(letter, E_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
    }

    for (int col = 0; col < Letter_size_col; col++) {
      for (int row = 0; row < Letter_size_row; row++) {
        if (letter[row][col] == 1) {
          leds[XY(arcade_start_pos_col - (Letter_size_col + 1) * current_letter - col, arcade_start_pos_row + row)] = CRGB::Blue;
        }
      }
    }
  }
}

void tetris_word(int start_pos_row, int start_pos_col) {
  for (int current_letter = 0; current_letter < tetris_length; current_letter++) { //itterating through each letter in the word we want to write.
    switch (current_letter) {
      case 0: memcpy(letter, T_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 1: memcpy(letter, E_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 2: memcpy(letter, T_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 3: memcpy(letter, R_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 4: memcpy(letter, I_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 5: memcpy(letter, S_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
    }

    for (int col = 0; col < Letter_size_col; col++) {
      for (int row = 0; row < Letter_size_row; row++) {
        if (letter[row][col] == 1) {
          leds[XY(start_pos_col - (Letter_size_col + 1) * current_letter - col, start_pos_row + row)] = CRGB::Red;
        }
      }
    }
  }
}

void seting_word(int start_pos_row, int start_pos_col) {
  for (int current_letter = 0; current_letter < seting_length; current_letter++) { //itterating through each letter in the word we want to write.
    switch (current_letter) {
      case 0: memcpy(letter, S_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 1: memcpy(letter, E_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 2: memcpy(letter, T_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 3: memcpy(letter, I_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 4: memcpy(letter, N_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 5: memcpy(letter, G_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
    }

    for (int col = 0; col < Letter_size_col; col++) {
      for (int row = 0; row < Letter_size_row; row++) {
        if (letter[row][col] == 1) {
          leds[XY(start_pos_col - (Letter_size_col + 1) * current_letter - col, start_pos_row + row)] = CRGB::Green;
        }
      }
    }
  }
}

void snake_word(int snake_start_pos_col, int snake_start_pos_row) {
  for (int current_letter = 0; current_letter < snake_length; current_letter++) { //itterating through each letter in the word we want to write.

    switch (current_letter) {
      case 0: memcpy(letter, S_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 1: memcpy(letter, N_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 2: memcpy(letter, A_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 3: memcpy(letter, K_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
      case 4: memcpy(letter, E_letter, Letter_size_row * Letter_size_col * sizeof(int)); break;
    }

    for (int col = 0; col < Letter_size_col; col++) {
      for (int row = 0; row < Letter_size_row; row++) {
        if (letter[row][col] == 1) {
          leds[XY(snake_start_pos_col - (Letter_size_col + 1) * current_letter - col, snake_start_pos_row + row)] = CRGB::Yellow;
        }
      }
    }
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

void home_screen()
{
  GameOver = true;
  choice  = "";
  clue_word(clue_start_pos_col, clue_start_pos_row);
  arcade_word(arcade_start_pos_col, arcade_start_pos_row);
  select_line(line1_start_pos_col, line1_length, line1_start_pos_row, scroll_move_home);
  snake_word(snake_start_pos_col, snake_start_pos_row);
  select_line(line2_start_pos_col, line2_length, line2_start_pos_row, scroll_move_home);
  tetris_word(tetris_start_pos_row, tetris_start_pos_col);
  seting_word(seting_home_start_pos_row, seting_home_start_pos_col);
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

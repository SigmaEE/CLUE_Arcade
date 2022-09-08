/*
  void game_selection() {
  int action = check_input();
  bool selection = false;
  if (selection == false) {
    Serial.println("First loop");
    action = check_input();
    /*
    if (action == ACT_NONE || action == ACT_L) { //home screen stays until up or down input on joystick
      action = check_input();
      game_selection();
      delay(1);
    }
    

    Serial.println("while loop over");
    if (action & ACT_U) {
      if (scroll_move > 0) {
        scroll_move -= (Letter_size_row + word_margin - 1);
        Serial.println("up");
        clearScreen();
        home_screen();
      }

    }
    if (action & ACT_D) {
      if (scroll_move < 2 * (Letter_size_row + word_margin - 1)) {
        scroll_move += (Letter_size_row + word_margin - 1);
        Serial.println("down");
        clearScreen();
        home_screen();
      }
    }

    if (action & ACT_R) {
      Serial.println("right");
      Serial.println("action is: ");
      Serial.println(action);
      Serial.println(selection);
      selection = true;
      Serial.println(selection);
      delay(1);
    }

  }
  else {
    Serial.println("Exited while loop");
    if (scroll_move == 0 * (Letter_size_row + word_margin - 1)) {
      game = "snake";
    } else {
      game = "tetris";
    }
    Serial.println("game selected");
    Serial.println(game);
    if (game == "snake") {
      snake_setup();
    } else if (game == "tetris") {
      tetris_setup();
    }
    Serial.println("setted up");
    GameOver = false;
  }
}
*/

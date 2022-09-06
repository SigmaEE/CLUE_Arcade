int letter[Letter_size_row][Letter_size_col];
int clue_length = 4;
int clue_start_pos_col = 24;
int clue_start_pos_row = 0;

int arcade_length = 6;
int arcade_start_pos_col = 29;
int arcade_start_pos_row = clue_start_pos_row + Letter_size_row + 1;

int tetris_length = 6;
int tetris_start_pos_col = 29;
int tetris_start_pos_row = kMatrixHeight - Letter_size_row - 1;

int snake_length = 5;
int snake_start_pos_col = 26;
int snake_start_pos_row = arcade_start_pos_row + Letter_size_row + 8;

int arrow_up_start_pos_col = kMatrixWidth/2 + Arrow_size_col/2;
int arrow_up_start_pos_row = snake_start_pos_row + Letter_size_row + 3;

int arrow_down_start_pos_col = kMatrixWidth/2 + Arrow_size_col/2;
int arrow_down_start_pos_row = tetris_start_pos_row - Arrow_size_row - 3;

int Arrow_up[Arrow_size_row][Arrow_size_col] = {  { 0, 0, 1, 0, 0},
                                                  { 0, 1, 1, 1, 0 },
                                                  { 1, 0, 1, 0, 1 },
                                                  { 0, 0, 1, 0, 0 },
                                                  { 0, 0, 1, 0, 0 }
                                                };

int Arrow_down[Arrow_size_row][Arrow_size_col] = {  { 0, 0, 1, 0, 0},
                                                    { 0, 0, 1, 0, 0 },
                                                    { 1, 0, 1, 0, 1 },
                                                    { 0, 1, 1, 1, 0 },
                                                    { 0, 0, 1, 0, 0 }
                                                  };                                                  


int A_letter[Letter_size_row][Letter_size_col] = {  { 0, 1, 1, 0 },
                                                    { 1, 0, 0, 1 },
                                                    { 1, 1, 1, 1 },
                                                    { 1, 0, 0, 1 },
                                                    { 1, 0, 0, 1 }
                                                  };

int C_letter[Letter_size_row][Letter_size_col] = {  { 0, 1, 1, 1 },
                                                    { 1, 0, 0, 0 },
                                                    { 1, 0, 0, 0 },
                                                    { 1, 0, 0, 0 },
                                                    { 0, 1, 1, 1 }
                                                  };


int D_letter[Letter_size_row][Letter_size_col] = {  { 1, 1, 1, 0 },
                                                    { 1, 0, 0, 1 },
                                                    { 1, 0, 0, 1 },
                                                    { 1, 0, 0, 1 },
                                                    { 1, 1, 1, 0 }
    };

int E_letter[Letter_size_row][Letter_size_col] = {  { 1, 1, 1, 1 },
                                                    { 1, 0, 0, 0 },
                                                    { 1, 1, 1, 0 },
                                                    { 1, 0, 0, 0 },
                                                    { 1, 1, 1, 1 }
                                                  };
                                                  
int I_letter[Letter_size_row][Letter_size_col] = {  { 1, 1, 1, 1 },
                                                    { 0, 1, 1, 0 },
                                                    { 0, 1, 1, 0 },
                                                    { 0, 1, 1, 0 },
                                                    { 1, 1, 1, 1 }
                                                  };                                                  

int K_letter[Letter_size_row][Letter_size_col] = {  { 1, 0, 0, 1 },
                                                    { 1, 0, 1, 0 },
                                                    { 1, 1, 0, 0 },
                                                    { 1, 0, 1, 0 },
                                                    { 1, 0, 0, 1 }
    };
    
int L_letter[Letter_size_row][Letter_size_col] = {  { 1, 0, 0, 0 },
                                                    { 1, 0, 0, 0 },
                                                    { 1, 0, 0, 0 },
                                                    { 1, 0, 0, 0 },
                                                    { 1, 1, 1, 1 }
                                                  };

int N_letter[Letter_size_row][Letter_size_col] = {  { 1, 0, 0, 1 },
                                                    { 1, 1, 0, 1 },
                                                    { 1, 1, 1, 1 },
                                                    { 1, 0, 1, 1 },
                                                    { 1, 0, 0, 1 }
                                                  };                                                   

int R_letter[Letter_size_row][Letter_size_col] = {  { 1, 1, 1, 0 },
                                                    { 1, 0, 0, 1 },
                                                    { 1, 1, 1, 0 },
                                                    { 1, 0, 1, 0 },
                                                    { 1, 0, 0, 1 }
    };

int S_letter[Letter_size_row][Letter_size_col] = {  { 1, 1, 1, 1 },
                                                    { 1, 0, 0, 0 },
                                                    { 1, 1, 1, 1 },
                                                    { 0, 0, 0, 1 },
                                                    { 1, 1, 1, 1 }
    };

int T_letter[Letter_size_row][Letter_size_col] = {  { 1, 1, 1, 1 },
                                                    { 1, 1, 1, 1 },
                                                    { 0, 1, 1, 0 },
                                                    { 0, 1, 1, 0 },
                                                    { 0, 1, 1, 0 }
                                                  };
int U_letter[Letter_size_row][Letter_size_col] = {  { 1, 0, 0, 1 },
                                                    { 1, 0, 0, 1 },
                                                    { 1, 0, 0, 1 },
                                                    { 1, 0, 0, 1 },
                                                    { 1, 1, 1, 1 }
    };




void clue_word(){

  
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

void arcade_word(){
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

void tetris_word(int start_pos_row, int start_pos_col, std::string color){
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

void snake_word(){
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

void arrow_up_sign(){ 

    for (int col = 0; col < Arrow_size_col; col++) {
      for (int row = 0; row < Arrow_size_row; row++) {
        if (Arrow_up[row][col] == 1) {
          leds[XY(arrow_up_start_pos_col - col, arrow_up_start_pos_row + row)] = CRGB::White;
        }
      }
    }
  }

void arrow_down_sign(){ 

    for (int col = 0; col < Arrow_size_col; col++) {
      for (int row = 0; row < Arrow_size_row; row++) {
        if (Arrow_down[row][col] == 1) {
          leds[XY(arrow_down_start_pos_col - col, arrow_down_start_pos_row + row)] = CRGB::White;
        }
      }
    }
  }

void home_screen() 
{
  delay(1000);
  clue_word();
  arcade_word();
  snake_word();
  arrow_up_sign();
  int tetris_start_pos_row2 = arrow_down_start_pos_row - (arrow_down_start_pos_row - Arrow_size_row - arrow_up_start_pos_row)/2 - (Letter_size_row/2);
  Serial.println(arrow_up_start_pos_row);
  Serial.println(Arrow_size_row);
  Serial.println(arrow_down_start_pos_row);
  int tetris_start_pos_col2 = tetris_start_pos_col;
  Serial.println(tetris_start_pos_row);
  Serial.println(tetris_start_pos_row2);
  tetris_word(tetris_start_pos_row2, tetris_start_pos_col2);
  arrow_down_sign();
  tetris_word(tetris_start_pos_row, tetris_start_pos_col);
  
  
  FastLED.show();
  delay(1500);
  Serial.println("Home screen");
}

///////////////////Snake Game

int fruitX, fruitY, score, count;
int dead_fruit_x[3];
int dead_fruit_y[3];
long int crash_timer_1 = -3000;
long int crash_timer_2 = -3000;
int score1, score2;
int X1, X2;
int Y1, Y2;
int speed_delay = 1000;
long int multi_tetris_timer;

// 1,2,3,4 down,left,up,right
int dir = ACT_L;
int dir2 = ACT_R;
int snake_action;
int tailX1[600], tailX2[600];
int tailY1[600], tailY2[600];
int nTail1 = 3;
int nTail2 = 3;

void fruitAnimation() {
  delay(100);
  for (int i = -1; i < 2; i++) {
    leds[XY(fruitX + i, fruitY + 1)] = CRGB::ForestGreen;
    leds[XY(fruitX + i, fruitY - 1)] = CRGB::ForestGreen;
  }
  leds[XY(fruitX - 1, fruitY)] = CRGB::ForestGreen;
  leds[XY(fruitX + 1, fruitY)] = CRGB::ForestGreen;
  FastLED.show();
  delay(100);

  for (int i = -2; i < 3; i++) {
    leds[XY(fruitX + i, fruitY + 2)] = CRGB::Green;
    leds[XY(fruitX + i, fruitY - 2)] = CRGB::Green;
  }
  for (int i = -1; i < 2; i++) {
    leds[XY(fruitX - 2, fruitY + i)] = CRGB::Green;
    leds[XY(fruitX + 2, fruitY + i)] = CRGB::Green;
  }
  FastLED.show();
  delay(100);

  for (int i = -3; i < 4; i++) {
    leds[XY(fruitX + i, fruitY + 3)] = CRGB::ForestGreen;
    leds[XY(fruitX + i, fruitY - 3)] = CRGB::ForestGreen;
  }
  for (int i = -2; i < 3; i++) {
    leds[XY(fruitX - 3, fruitY + i)] = CRGB::ForestGreen;
    leds[XY(fruitX + 3, fruitY + i)] = CRGB::ForestGreen;
  }
  FastLED.show();
  delay(100);
}

void snake_setup() {
  GameOver = false;
  memset(tailX1, 0, sizeof(tailX1));
  memset(tailY1, 0, sizeof(tailY1));
  dir = ACT_R;
  fruitX = rand() % w;
  fruitY = random(start_h, end_h);
  score = 0;
  X1 = 15;
  Y1 = 30;
  X2 = 100;
  Y2 = 100;
  memset(tailX2, 0, sizeof(tailX2));
  memset(tailY2, 0, sizeof(tailY2));
}

void inPut() {
  snake_action = check_joystick_input();
  if (active_player == 1){
    if (snake_action & ACT_L_P1 && !(dir & ACT_R)){
      dir = ACT_L;
    } else if (snake_action & ACT_R_P1 && !(dir & ACT_L)) {
      dir = ACT_R;
    } else if (snake_action & ACT_D_P1 && !(dir & ACT_U)) {
      dir = ACT_D;
    } else if (snake_action & ACT_U_P1 && !(dir & ACT_D)) {
      dir = ACT_U;
    } else if (snake_action & ACT_B1_P1) {
      //COOL FUNTION
    } else if (snake_action & ACT_B2_P1) {
      GameOver = true;
    }
  }
  else{
    if (snake_action & ACT_L_P2 && !(dir & ACT_R)){
      dir = ACT_L;
    } else if (snake_action & ACT_R_P2 && !(dir & ACT_L)) {
      dir = ACT_R;
    } else if (snake_action & ACT_D_P2 && !(dir & ACT_U)) {
      dir = ACT_D;
    } else if (snake_action & ACT_U_P2 && !(dir & ACT_D)) {
      dir = ACT_U;
    } else if (snake_action & ACT_B1_P2) {
      //COOL FUNTION
    } else if (snake_action & ACT_B2_P2) {
      GameOver = true;
    }
  }
}

void logic() {
  int prevX1 = tailX1[0];
  int prevY1 = tailY1[0];
  int prev2X1, prev2Y1;
  tailX1[0] = X1;
  tailY1[0] = Y1;

  for ( int i =  1; i < nTail1; i++) {
    prev2X1 = tailX1[i];
    prev2Y1 = tailY1[i];
    tailX1[i] = prevX1;
    tailY1[i] = prevY1;
    prevX1 = prev2X1;
    prevY1 = prev2Y1;
  }

  if (dir == ACT_D) {
    if (Y1 < h - 1) {
      Y1++;
    } else {
      Y1 = start_h;
    }
  } else if (dir == ACT_U) {
    if (Y1 > start_h) {
      Y1--;
    } else {
      Y1 = kMatrixHeight - 1;
    }
  } else if (dir == ACT_L) {
    if (X1 < w - 1) {
      X1++;
    } else {
      X1 = 0;
    }
  } else if (dir == ACT_R) {
    if (X1 > 0) {
      X1--;
    } else {
      X1 = 29;
    }
  }

  for (int i = 0; i < nTail1; i++) {
    if (tailX1[i] == X1 && tailY1[i] == Y1) {
      GameOver = true;
    }
  }

  if (X1 == fruitX && Y1 == fruitY) {
    score++;
    nTail1++;
    fruitAnimation();
    fruitX = rand() % w;
    fruitY = random(start_h, end_h);
  }
}

void draw() {
  clearScreen();
  for ( int i = 0; i < w; i++) {
    leds[XY(i, start_h - 1)] = CRGB::White;
    for (int j = start_h; j < end_h; j++) {
      if ( i == X1 && j == Y1) {
        leds[XY(i, j)] = CRGB::Purple;
      }
      else if ( i == fruitX && j == fruitY) {
        leds[XY(i, j)] = CRGB::Green;
      } 
      else {
        for (int k = 0; k < nTail1; k++) {
          if (tailX1[k] == i && tailY1[k] == j) {
            leds[XY(i, j)] = CRGB::Blue;
          }
        }
      }
    }
  }

  unsigned int tmp1 = score;
  int i = 0;
  while (tmp1 > 0) {
    if (tmp1 & 1) {
      leds[XY(0, i)] = CRGB::Blue;
    } else {
      leds[XY(0, i)] = CRGB::Red;
    }
    i++;
    tmp1 = tmp1 >> 1;
  }
  tmp1 = read_high_score_top("s");
  i = 0;
  while (tmp1 > 0) {
    if (tmp1 & 1) {
      leds[XY(29, i)] = CRGB::Blue;
    } else {
      leds[XY(29, i)] = CRGB::Red;
    }
    i++;
    tmp1 = tmp1 >> 1;
  }

  FastLED.show();
}

void snake_loop()
{
  if (GameOver != true) {
    draw();
    inPut();
    logic();
    delay(5);
  }

  if (GameOver == true) {
    if (score > read_high_score_low("t")) {
      struct hs_player player;
      new_hs_player(score, player);
      write_high_score("s", player, curr_snake_board);
      hsAnimation();
      unsigned int tmp = read_high_score_top("s"); // WHAT IS THIS FOR?
      int i = 0;
      while (tmp > 0) {
        if (tmp & 1) {
          leds[XY(29, i)] = CRGB::Blue;
        } else {
          leds[XY(29, i)] = CRGB::Red;
        }
        i++;
        tmp = tmp >> 1;
      }
    }
    deathAnimation();
    switch_screen(&home_screen);

    for (int j = 0; j < kMatrixHeight; j++) {
      for ( int i = 0; i < kMatrixWidth; i++) {
        leds[XY(i, j)] = CRGB::Red;
      }
    }

    FastLED.show();
    delay(150);
    clearScreen();
    FastLED.show();
    delay(150);
    for (int j = 0; j < kMatrixHeight; j++) {
      for ( int i = 0; i < kMatrixWidth; i++) {
        leds[XY(i, j)] = CRGB::Red;
      }
    }

    FastLED.show();
    delay(150);
    clearScreen();
    FastLED.show();

    nTail1 = 3;
  }
}



void snake_multi_setup(){
  GameOver = false;
  memset(tailX1, 0, sizeof(tailX1));
  memset(tailY1, 0, sizeof(tailY1));
  memset(tailX2, 0, sizeof(tailX2));
  memset(tailY2, 0, sizeof(tailY2));
  dir = ACT_L;
  dir2 = ACT_R;
  fruitX = rand() % w;
  fruitY = random(start_h, end_h);
  score1 = 0;
  score2 = 0;
  nTail1 = 10;
  nTail2 = 10;
  X1 = 20;
  Y1 = 35;
  X2 = 10;
  Y2 = 25;
  multi_tetris_timer = millis();
}

void multi_inPut() {
  snake_action = check_joystick_input();
  static int ticks2 = 0;
  static int ticks1 = 0;
  
  if (snake_action & ACT_L_P1 && (dir != ACT_R)){
    dir = ACT_L;
  } else if (snake_action & ACT_R_P1 && (dir != ACT_L)) {
    dir = ACT_R;
  } else if (snake_action & ACT_D_P1 && (dir != ACT_U)) {
    dir = ACT_D;
  } else if (snake_action & ACT_U_P1 && (dir != ACT_D)) {
    dir = ACT_U;
  } else if (snake_action & ACT_B1_P1) {
    //COOL FUNCTION
  } else if (snake_action & ACT_B2_P1) {
    ticks1++;
    if(ticks1 > 8){
      ticks1 = 0;
      GameOver = true;  
    }
  }
  if (snake_action & ACT_L_P2 && (dir2 != ACT_R)){
    dir2 = ACT_L;
  } else if (snake_action & ACT_R_P2 && (dir2 != ACT_L)) {
    dir2 = ACT_R;
  } else if (snake_action & ACT_D_P2 && (dir2 != ACT_U)) {
    dir2 = ACT_D;
  } else if (snake_action & ACT_U_P2 && (dir2 != ACT_D)) {
    dir2 = ACT_U;
  } else if (snake_action & ACT_B1_P2) {
    // COOL FUNCTION
  } else if (snake_action & ACT_B2_P2) {
    ticks2 ++;
    if (ticks2 > 8){
      ticks2 = 0;
      GameOver = true; 
    }
  }
}

void multi_logic() {
  int prevX1 = tailX1[0];
  int prevX2 = tailX2[0];
  int prevY1 = tailY1[0];
  int prevY2 = tailY2[0];
  int prev2X1, prev2Y1;
  int prev2X2, prev2Y2;
  tailX1[0] = X1;
  tailX2[0] = X2;
  tailY1[0] = Y1;
  tailY2[0] = Y2;

  for ( int i =  1; i < nTail1; i++) {
    prev2X1 = tailX1[i];
    prev2Y1 = tailY1[i];
    tailX1[i] = prevX1;
    tailY1[i] = prevY1;
    prevX1 = prev2X1;
    prevY1 = prev2Y1;
  }

  for ( int i =  1; i < nTail2; i++) {
    prev2X2 = tailX2[i];
    prev2Y2 = tailY2[i];
    tailX2[i] = prevX2;
    tailY2[i] = prevY2;
    prevX2 = prev2X2;
    prevY2 = prev2Y2;
  }

  if (dir == ACT_D) {
    if (Y1 < h - 1) {
      Y1++;
    } else {
      Y1 = start_h;
    }
  } else if (dir == ACT_U) {
    if (Y1 > start_h) {
      Y1--;
    } else {
      Y1 = kMatrixHeight - 1;
    }
  } else if (dir == ACT_L) {
    if (X1 < w - 1) {
      X1++;
    } else {
      X1 = 0;
    }
  } else if (dir == ACT_R) {
    if (X1 > 0) {
      X1--;
    } else {
      X1 = 29;
    }
  }

  if (dir2 == ACT_D) {
    if (Y2 < h - 1) {
      Y2++;
    } else {
      Y2 = start_h;
    }
  } else if (dir2 == ACT_U) {
    if (Y2 > start_h) {
      Y2--;
    } else {
      Y2 = kMatrixHeight - 1;
    }
  } else if (dir2 == ACT_L) {
    if (X2 < w - 1) {
      X2++;
    } else {
      X2 = 0;
    }
  } else if (dir2 == ACT_R) {
    if (X2 > 0) {
      X2--;
    } else {
      X2 = 29;
    }
  }

  if (millis() - crash_timer_1 > 3000){
    for (int i = 0; i < nTail1; i++) {
      if (tailX1[i] == X1 && tailY1[i] == Y1) {
        int tail_lost1 = 0;
        if (score1 >= 3){
          score1 -= 3;
          tail_lost1 = 3;
        }
        else {
          tail_lost1 = score1;
          score1 = 0;
        }
        for (int j = 0; j < tail_lost1; j++){
          dead_fruit_x[j] = tailX1[j];
          dead_fruit_y[j] = tailY1[j];  
        }
        nTail1 += 3;
        crash_timer_1 = millis();
      }
    }
  }

  if (millis() - crash_timer_2 > 3000){
    for (int i = 0; i < nTail2; i++) {
      if (tailX2[i] == X2 && tailY2[i] == Y2) {
        int tail_lost2 = 0;
        if (score2 >= 3){
          tail_lost2 = 3;
          score2 -= 3;
        }
        else{
          tail_lost2 = score2;
          score2 = 0;
        }
        for (int j = 0; j < tail_lost2; j++){
          dead_fruit_x[j] = tailX2[j];
          dead_fruit_y[j] = tailY2[j];
        }
        
        nTail2 += 3;
        crash_timer_2 = millis();
      }
    }
  }

  for (int z = 0; z < 3; z++){
    if(X2 == dead_fruit_x[z] && Y2 == dead_fruit_y[z]){
      score2++;
      nTail2++;
      dead_fruit_x[z] = 0;
      dead_fruit_y[z] = 0;
    }
  }

  for (int z = 0; z < 3; z++){
    if(X1 == dead_fruit_x[z] && Y1 == dead_fruit_y[z]){
      score1++;
      nTail1++;
      dead_fruit_x[z] = 0;
      dead_fruit_y[z] = 0;
    } 
  }

  if (X1 == fruitX && Y1 == fruitY) {
    score1++;
    nTail1++;
    fruitAnimation();
    fruitX = rand() % w;
    fruitY = random(start_h, end_h);
  }
  if(X2 == fruitX && Y2 == fruitY){
    score2++;
    nTail2++;
    fruitAnimation();
    fruitX = rand() % w;
    fruitY = random(start_h, end_h);
  }
}

void multi_draw() {
  clearScreen();
  int blue_temp;
  int red_temp;
  if (dir == ACT_R || dir == ACT_U){
    blue_temp = 255;
  }
  else{
    blue_temp = 6;
  }
  if (dir2 == ACT_R || dir2 == ACT_U){
    red_temp = 255;
  }
  else{
    red_temp = 6;
  }
  int blue_factor = 255 / nTail1;
  int red_factor = 255 / nTail2;
  for ( int i = 0; i < w; i++) {
    leds[XY(i, start_h - 1)] = CRGB::White;
    for (int j = start_h; j < end_h; j++) {
      if ( i == X1 && j == Y1) {
        leds[XY(i, j)] = CRGB::Purple;
      }
      else if ( i == X2 && j == Y2) {
        leds[XY(i, j)] = CRGB::Purple;
      }
      else if ( i == fruitX && j == fruitY) {
        leds[XY(i, j)] = CRGB::Green;
      }
      else if ((i == dead_fruit_x[0] && j == dead_fruit_y[0]) || 
               (i == dead_fruit_x[1] && j == dead_fruit_y[1]) ||
               (i == dead_fruit_x[2] && j == dead_fruit_y[2])){
        leds[XY(i,j)] = CRGB::Yellow;
       }
      else {
        for (int k = 0; k < nTail1; k++) {
          if (tailX1[k] == i && tailY1[k] == j) {
            if (millis() - crash_timer_1 < 3000){
              leds[XY(i, j)] = 0x000006;
            }
            else{
              leds[XY(i, j)] = ((0 & 0xff) << 16) + ((0 & 0xff) << 8) + (blue_temp & 0xff);
              if (dir == ACT_R || dir == ACT_U){
                blue_temp -= blue_factor;  
              }
              else{
                blue_temp += blue_factor;
              }
            }
          }
        }
        for (int k = 0; k < nTail2; k++) {
          if (tailX2[k] == i && tailY2[k] == j) {
            if (millis() - crash_timer_2 < 3000){
              leds[XY(i,j)] = 0x060000;   
            }
            else {
              leds[XY(i, j)] = ((red_temp & 0xff) << 16) + ((0 & 0xff) << 8) + (0 & 0xff);
              if (dir2 == ACT_R || dir2 == ACT_U){
                red_temp -= red_factor;  
              }
              else{
                red_temp += red_factor;
              }
            }
          }
        }
      }
    }
  }
  int i = 0;
  int green = 0x0caf00;
  int orange = 0xff7300;
  int white = 0xffffff;
  int red = 0xff0000;
  int blue = 0x0000ff;
  int remaining_time = 150 - ((millis() - multi_tetris_timer)/1000);
  if (remaining_time <= 0){
    GameOver = true;
  }
  draw_color_xpm(white, numbers[(remaining_time/100) % 10], 20, 0);
  draw_color_xpm(white, numbers[(remaining_time/10) % 10], 16, 0);
  draw_color_xpm(white, numbers[remaining_time % 10], 12, 0);
  draw_color_xpm(blue, numbers[score1 % 10],25,7);
  draw_color_xpm(blue, numbers[(score1/10) % 10],29,7);
  draw_color_xpm(red, numbers[score2 % 10],2,7);
  draw_color_xpm(red, numbers[(score2/10) % 10],6,7);

  FastLED.show();
}

void snake_multi_loop()
{
  if (GameOver != true) {
    multi_draw();
    multi_inPut();
    multi_logic();
    delay(5);
  }
  else{
    clearScreen();
    for (int k = 0; k < 2; k++){
      for (int j = 0; j < kMatrixHeight; j++) {
        for ( int i = 0; i < kMatrixWidth; i++) {
          leds[XY(i, j)] = CRGB::Red;
        }
      }
      FastLED.show();
      delay(150);
      clearScreen();
      FastLED.show();
      delay(150);
    }

    int color = 0x00FF00;
    if (score2 > score1){
      color = 0xFF0000;
      draw_word(22, 25, (char*)"RED", 0xFFFFFF);
      draw_word(24, 32, (char*)"WINS", 0xFFFFFF);  
    }
    else if (score1 > score2){
      color = 0x0000FF;
      draw_word(24, 25, (char*)"BLUE", 0xFFFFFF);
      draw_word(24, 32, (char*)"WINS", 0xFFFFFF);
    }
    else{
      draw_word(24, 25, (char*)"DRAW", 0xFFFFFF);
    }
    
    fire_work(7,20,7,12,color);
    fire_work(15,20,15,10,color);
    fire_work(23,20,23,12,color);
    
    delay(4000);
   
    switch_screen(&home_screen);
  }
}

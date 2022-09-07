///////////////////Snake Game


int fruitX, fruitY, score, count;
int speed_delay = 1000;

// 1,2,3,4 down,left,up,right
int dir = ACT_L;

int tailX[600];
int tailY[600];
int nTail = 3;

void inPut() {

  xPotVal = analogRead(xPotPin);
  yPotVal = analogRead(yPotPin);

  //Serial.print("xval = ");

  //Serial.println(xPotVal);

  //Serial.print("yval = ");

  //Serial.println(yPotVal);


  if ( xPotVal < 400) {

    dir = ACT_L;

  } else if (xPotVal > 600) {
    
    dir = ACT_R;
  }
  if (yPotVal < 400) {
    
    dir = ACT_D;
    
  } else if (yPotVal > 600) {
    
    dir = ACT_U;
  }
}
void fruitAnimation() {
  Serial.println("pre animation");

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



  Serial.println("animation");
}

void logic() {
  int prevX = tailX[0];
  int prevY = tailY[0];
  int prev2X, prev2Y;
  tailX[0] = x;
  tailY[0] = y;

  for ( int i =  1; i < nTail; i++) {
    prev2X = tailX[i];
    prev2Y = tailY[i];
    tailX[i] = prevX;
    tailY[i] = prevY;
    prevX = prev2X;
    prevY = prev2Y;
  }

  if (dir == ACT_D) {
    if (y < h - 1) {
      y++;
    } else {
      y = 0;
    }
  } else if (dir == ACT_U) {
    if (y > 0) {
      y--;
    } else {
      y = kMatrixHeight - 1;
    }
  } else if (dir == ACT_L) {
    if (x < w - 1) {
      x++;
    } else {
      x = 0;
    }
  }
  else if (dir == ACT_R) {
    if (x > 0) {
      x--;
    } else {
      x = 29;
    }
  }

  for (int i = 0; i < nTail; i++) {
    if (tailX[i] == x && tailY[i] == y) {
      GameOver = true;
    }
  }

  if (x == fruitX && y == fruitY) {
    score++;
    nTail++;
    fruitAnimation();
    fruitX = rand() % w;
    fruitY = rand() % h;
  }
}

void draw() {
  clearScreen();
  for ( int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      if ( i == x && j == y) {
        leds[XY(i, j)] = CRGB::Blue;
      } else if ( i == fruitX && j == fruitY) {
        leds[XY(i, j)] = CRGB::Green;
      } else {
        for (int k = 0; k < nTail; k++) {
          if (tailX[k] == i && tailY[k] == j) {
            leds[XY(i, j)] = CRGB::Purple;
          }
        }
        // leds[XY(tailX[nTail], tailY[nTail])] = CRGB::Black;
      }
    }
  }

  FastLED.show();

  //  Serial.print("xpos = ");
  //  Serial.println(x);
  //  Serial.print("ypos = ");
  //  Serial.println(y);
}

void snake_setup() {
  GameOver = false;
  fruitX = rand() % w;
  fruitY = rand() % h;
  score = 0;
}

void snake_loop() {
  while (GameOver == false) {
    //  Serial.println("Game is running");
    draw();
    inPut();
    logic();
    delay(5);
  }

  if(GameOver == true) {

    deathAnimation();

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

    //
    //    for (int j = 0; j < kMatrixHeight; j++) {
    //      for ( int i = 0; i < kMatrixWidth; i++) {
    //
    //        leds[XY(i, j)] = CRGB::Black;
    //      }
    //      FastLED.show();
    //    }
  
    //GameOver = false;
    nTail = 3;
  }
  //
  //FastLED.show();
  //delay(30);
}

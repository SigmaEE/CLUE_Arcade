///////////////////Snake Game


int fruitX, fruitY, score, count;
int speed_delay = 1000;

// 1,2,3,4 up,right,down,left
int dir = 2;

int tailX[600];
int tailY[600];
int nTail = 40;

void inPut() {

  xPotVal = analogRead(xPotPin);
  yPotVal = analogRead(yPotPin);

  Serial.print("xval = ");

  Serial.println(xPotVal);

  Serial.print("yval = ");

  Serial.println(yPotVal);


  if ( xPotVal < 400) {
    //    if(dir = 4){
    //      gameOver = true;
    //    }
    dir = 4;
  } else if (xPotVal > 600) {
    //        if(dir = 2){
    //      gameOver = true;
    //    }
    dir = 2;
  }
  if (yPotVal < 400) {
    //        if(dir = 1){
    //      gameOver = true;
    //    }
    dir = 3;
  } else if (yPotVal > 600) {
    //            if(dir = 3){
    //      gameOver = true;
    //    }
    dir = 1;
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

  if (dir == 1) {
    if (y < h - 1) {
      y++;
    } else {
      y = 0;
    }
  } else if (dir == 3) {
    if (y > 0) {
      y--;
    } else {
      y = kMatrixHeight-1;
    }
  } else if (dir == 2) {
    if (x < w - 1) {
      x++;
    } else {
      x = 0;
    }
  }
  else if (dir == 4) {
    if (x > 0) {
      x--;
    } else {
      x = 29;
    }
  }

  for (int i = 0; i < nTail; i++) {
    if (tailX[i] == x && tailY[i] == y) {
      gameOver = true;
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
  
  gameOver = false;
  fruitX = rand() % w;
  fruitY = rand() % h;
  score = 0;
}

void snake_loop() {
  while (!gameOver) {
    //  Serial.println("Game is running");
    draw();
    inPut();
    logic();
    delay(5);
  }

  while (gameOver) {

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

    gameOver = false;
    nTail = 0;
  }
  //
  //FastLED.show();
  //delay(30);
}

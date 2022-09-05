///// LED Related
#include <FastLED.h>

#define LED_PIN  3

#define COLOR_ORDER GRB
#define CHIPSET     WS2812B

#define BRIGHTNESS 64

// Params for width and height
const uint8_t kMatrixWidth = 30;
const uint8_t kMatrixHeight = 58;

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);



uint16_t XY( uint8_t x, uint8_t y)
{

  
  uint16_t i;

if( !( x<0 || x >= kMatrixWidth || y<0 || y >= kMatrixHeight )){

  if ( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if ( kMatrixSerpentineLayout == true) {
    if ( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }

  return i;
}

return 2000;
}

void clearScreen() {
  for ( int i = 0; i < kMatrixWidth; i++) {
    for (int j = 0; j < kMatrixHeight; j++) {
      leds[XY(i, j)] = CRGB::Black;
    }
  }
  //FastLED.show();
}


///////////////////Snake Game

int xPotVal;
int yPotVal;
const int xPotPin = A0;
const int yPotPin = A1;


bool gameOver;
const int w = kMatrixWidth;
const int h = kMatrixHeight;

int x, y,  fruitX, fruitY, score;

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

void deathAnimation() {
  Serial.println("pre animation");

  delay(100);
  for (int i = -1; i < 2; i++) {
    leds[XY(x + i, y + 1)] = CRGB::Red;
    leds[XY(x + i, y - 1)] = CRGB::Red;
  }
  leds[XY(x - 1, y)] = CRGB::Red;
  leds[XY(x + 1, y)] = CRGB::Red;
  FastLED.show();
  delay(100);


  for (int i = -2; i < 3; i++) {
    leds[XY(x + i, y + 2)] = CRGB::Red;
    leds[XY(x + i, y - 2)] = CRGB::Red;
  }
  for (int i = -1; i < 2; i++) {
    leds[XY(x - 2, y + i)] = CRGB::Red;
    leds[XY(x + 2, y + i)] = CRGB::Red;
  }

  FastLED.show();
  delay(100);


  for (int i = -3; i < 4; i++) {
    leds[XY(x + i, y + 3)] = CRGB::Red;
    leds[XY(x + i, y - 3)] = CRGB::Red;
  }
  for (int i = -2; i < 3; i++) {
    leds[XY(x - 3, y + i)] = CRGB::Red;
    leds[XY(x + 3, y + i)] = CRGB::Red;
  }

  FastLED.show();
  delay(100);



  Serial.println("animation");
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

void setup() {
  Serial.begin(9600);
  Serial.println("Setting up");
  gameOver = false;
  x = w / 2;
  y = h / 2;

  fruitX = rand() % w;
  fruitY = rand() % h;
  score = 0;



  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  clearScreen();
  FastLED.show();

  // leds[XY(tailX[nTail], tailY[nTail])] = CRGB::Black;

}

void loop() {
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

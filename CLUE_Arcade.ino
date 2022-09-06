///// LED Related
#include <FastLED.h>
#include <EEPROM.h>
#define LED_PIN  3

#define COLOR_ORDER GRB
#define CHIPSET     WS2812B

#define BRIGHTNESS 64

// Params for width and height
const uint8_t kMatrixWidth = 30;
const uint8_t kMatrixHeight = 58;

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;

int xPotVal;
int yPotVal;
const int xPotPin = A0;
const int yPotPin = A1;

const int w = kMatrixWidth;
const int h = kMatrixHeight;

bool gameOver;
int x, y,  highScore;


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

uint8_t readHighScore() {
  static uint8_t tmpScore = 0;
  static uint8_t memHighScore = 0;
  
  for (int i=0; i<1024; i++) {
    tmpScore = EEPROM.read(i);
    if (tmpScore > memHighScore) {
      memHighScore = tmpScore;
    }
  }
  return memHighScore;
}

void writeHighScore() {
  static uint8_t tmpAdr = 0;
  while (EEPROM.read(tmpAdr) != 0) {
    tmpAdr++;
  }
  EEPROM.write(tmpAdr, highScore);
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

void hsAnimation() {
  Serial.println("pre animation");

  delay(100);
  for (int y = 0; y < 57; ++y) {
    for (int x = 0; x < 29; ++x) {
      leds[XY(x,y)] = CRGB::Green;
      }
  }
  delay(100);

  Serial.println("animation");
}

void setup() {
  Serial1.begin(9600);
  Serial1.println("Setting up");
  
  x = w / 2;
  y = h / 2;

  highScore = readHighScore();
  Serial1.println(highScore);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  clearScreen();
  FastLED.show();

  // leds[XY(tailX[nTail], tailY[nTail])] = CRGB::Black;

  snake_setup();
  tetris_setup();
}

void loop() {
  // snake_loop();
  tetris_loop();

}

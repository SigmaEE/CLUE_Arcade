void fire_work(int start_x, int start_y, int end_x, int end_y, int color){
// Fade the LED out over time as it travels up the display
  int r = ((color >> 16) & 0xFF);
  int g = ((color >> 8) & 0xFF);
  int b = (color & 0xFF);
  for (int y = start_y; y >= end_y; y--) {
    //clearScreen();
    leds[XY(start_x,y)] = CRGB(100, 100, 0);
    leds[XY(start_x,y+1)] = CRGB(50, 50, 0);
    leds[XY(start_x,y+2)] = CRGB(25, 25, 0);
    leds[XY(start_x,y+3)] = CRGB(10, 10, 0);
    FastLED.show();
    delay(5);
    leds[XY(start_x,y)] = CRGB(0, 0, 0);
    leds[XY(start_x,y+1)] = CRGB(0, 0, 0);
    leds[XY(start_x,y+2)] = CRGB(0, 0, 0);
    leds[XY(start_x,y+3)] = CRGB(0, 0, 0);
  }
  //clearScreen();

  leds[XY(end_x, end_y)] = CRGB(r,g,b);
  FastLED.show();
  delay(20);
  
  leds[XY(end_x,end_y)] = CRGB(r/10, g/10, b/10);
  leds[XY(end_x+1, end_y+1)] = CRGB(r,g,b);
  leds[XY(end_x-1, end_y-1)] = CRGB(r,g,b);
  leds[XY(end_x-1, end_y+1)] = CRGB(r,g,b);
  leds[XY(end_x+1, end_y-1)] = CRGB(r,g,b);

  leds[XY(end_x+1, end_y)] = CRGB(r,g,b);
  leds[XY(end_x-1, end_y)] = CRGB(r,g,b);
  leds[XY(end_x, end_y+1)] = CRGB(r,g,b);
  leds[XY(end_x, end_y-1)] = CRGB(r,g,b);
  FastLED.show();
  delay(20);

  leds[XY(end_x,end_y)] = CRGB(r/24,g/24,b/24);
  
  leds[XY(end_x+1, end_y+1)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x-1, end_y-1)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x-1, end_y+1)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x+1, end_y-1)] = CRGB(r/10,g/10,b/10);

  leds[XY(end_x+1, end_y)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x-1, end_y)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x, end_y+1)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x, end_y-1)] = CRGB(r/10,g/10,b/10);

  leds[XY(end_x+2, end_y+2)] = CRGB(r,g,b);
  leds[XY(end_x-2, end_y-2)] = CRGB(r,g,b);
  leds[XY(end_x-2, end_y+2)] = CRGB(r,g,b);
  leds[XY(end_x+2, end_y-2)] = CRGB(r,g,b);

  leds[XY(end_x+2, end_y)] = CRGB(r,g,b);
  leds[XY(end_x-2, end_y)] = CRGB(r,g,b);
  leds[XY(end_x, end_y+2)] = CRGB(r,g,b);
  leds[XY(end_x, end_y-2)] = CRGB(r,g,b);
  FastLED.show();
  delay(20);
  
  leds[XY(end_x+1, end_y+1)] = CRGB(r/24,g/24,b/24);
  leds[XY(end_x-1, end_y-1)] = CRGB(r/24,g/24,b/24);
  leds[XY(end_x-1, end_y+1)] = CRGB(r/24,g/24,b/24);
  leds[XY(end_x+1, end_y-1)] = CRGB(r/24,g/24,b/24);

  leds[XY(end_x+1, end_y)] = CRGB(r/24,g/24,b/24);
  leds[XY(end_x-1, end_y)] = CRGB(r/24,g/24,b/24);
  leds[XY(end_x, end_y+1)] = CRGB(r/24,g/24,b/24);
  leds[XY(end_x, end_y-1)] = CRGB(r/24,g/24,b/24);

  leds[XY(end_x+2, end_y+2)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x-2, end_y-2)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x-2, end_y+2)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x+2, end_y-2)] = CRGB(r/10,g/10,b/10);

  leds[XY(end_x+2, end_y)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x-2, end_y)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x, end_y+2)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x, end_y-2)] = CRGB(r/10,g/10,b/10);

  leds[XY(end_x+3, end_y+3)] = CRGB(r,g,b);
  leds[XY(end_x-3, end_y-3)] = CRGB(r,g,b);
  leds[XY(end_x-3, end_y+3)] = CRGB(r,g,b);
  leds[XY(end_x+3, end_y-3)] = CRGB(r,g,b);

  leds[XY(end_x+3, end_y)] = CRGB(r,g,b);
  leds[XY(end_x-3, end_y)] = CRGB(r,g,b);
  leds[XY(end_x, end_y+3)] = CRGB(r,g,b);
  leds[XY(end_x, end_y-3)] = CRGB(r,g,b);
  FastLED.show();
  delay(20);

  leds[XY(end_x+2, end_y+2)] = CRGB(r/24,g/24,b/24);
  leds[XY(end_x-2, end_y-2)] = CRGB(r/24,g/24,b/24);
  leds[XY(end_x-2, end_y+2)] = CRGB(r/24,g/24,b/24);
  leds[XY(end_x+2, end_y-2)] = CRGB(r/24,g/24,b/24);

  leds[XY(end_x+2, end_y)] = CRGB(r/24,g/24,b/24);
  leds[XY(end_x-2, end_y)] = CRGB(r/24,g/24,b/24);
  leds[XY(end_x, end_y+2)] = CRGB(r/24,g/24,b/24);
  leds[XY(end_x, end_y-2)] = CRGB(r/24,g/24,b/24);

  leds[XY(end_x+3, end_y+3)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x-3, end_y-3)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x-3, end_y+3)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x+3, end_y-3)] = CRGB(r/10,g/10,b/10);

  leds[XY(end_x+3, end_y)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x-3, end_y)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x, end_y+3)] = CRGB(r/10,g/10,b/10);
  leds[XY(end_x, end_y-3)] = CRGB(r/10,g/10,b/10);

  leds[XY(end_x+4, end_y+4)] = CRGB(r,g,b);
  leds[XY(end_x-4, end_y-4)] = CRGB(r,g,b);
  leds[XY(end_x-4, end_y+4)] = CRGB(r,g,b);
  leds[XY(end_x+4, end_y-4)] = CRGB(r,g,b);

  leds[XY(end_x+4, end_y)] = CRGB(r,g,b);
  leds[XY(end_x-4, end_y)] = CRGB(r,g,b);
  leds[XY(end_x, end_y+4)] = CRGB(r,g,b);
  leds[XY(end_x, end_y-4)] = CRGB(r,g,b);
  FastLED.show();
  delay(20);
}

#pragma once

#include <Adafruit_NeoPixel.h>

class RGBLED {
public:
  RGBLED();
  void set(uint8_t r,uint8_t g,uint8_t b);
  void step();
private:
  Adafruit_NeoPixel _led;
  uint8_t _r,_g,_b;
};
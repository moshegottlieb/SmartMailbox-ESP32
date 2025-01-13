#include "RGBLED.h"

RGBLED::RGBLED():_led(1,8,NEO_GRB + NEO_KHZ800){
  _led.begin();
  _led.show();
  set(0,0,0);
}

void RGBLED::set(uint8_t r,uint8_t g,uint8_t b){
  _r = r; _g = g; _b = b;
  //g,r,b
  _led.setPixelColor(0,g,r,b);
  _led.show();
}

void RGBLED::step(){
  set(_r+1,_g+1,_b+1);
}


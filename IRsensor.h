#pragma once

#include <stdint.h>

class IRSensor {
public:
  IRSensor(uint8_t power,uint8_t signal);
  bool proximity();
private:
  uint8_t _power;
  uint8_t _signal;
};
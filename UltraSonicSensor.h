#pragma once

#include <stdint.h>

/**
* @class UltraSonicSensor
* A wrapper for HC-SR04 ultra sonic sensor
*/
class UltraSonicSensor {
public:
  /**
  * Set the echo pin and trigger pin numbers
  */
  UltraSonicSensor(uint8_t echo,uint8_t trigger);
  float read() const;
private:
  uint16_t _scale;
  uint8_t _echo;
  uint8_t _trigger;
};
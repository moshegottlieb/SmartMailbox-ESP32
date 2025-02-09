#include "IRsensor.h"
#include <Arduino.h>

IRSensor::IRSensor(uint8_t power,uint8_t signal):_power(power),_signal(signal){
  pinMode(_power,OUTPUT);
  pinMode(_signal,INPUT);
  digitalWrite(_power,LOW); // turn off
}

bool IRSensor::proximity(){
  digitalWrite(_power,HIGH);
  delay(10);
  bool ret = digitalRead(_signal) == LOW;
  digitalWrite(_power,LOW);
  return ret;
}
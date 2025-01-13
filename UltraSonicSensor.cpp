#include "UltraSonicSensor.h"
#include <Arduino.h>

UltraSonicSensor::UltraSonicSensor(uint8_t echo,uint8_t trigger):_echo(echo),_trigger(trigger){
  pinMode(echo,INPUT);
  pinMode(trigger,OUTPUT);
}

float UltraSonicSensor::read() const{
  digitalWrite(_trigger,LOW);
  delay(2); // Make sure the pin is low, maybe we can skip this
  digitalWrite(_trigger,HIGH);
  delay(10); // trigger sound for 10ms
  digitalWrite(_trigger, LOW); // turn off sound
  // How much time did it take? microseconds (unsigned long), but we convert it to float
  float timing = pulseIn(_echo,HIGH);
  // Speed=distance/time, so time*speed=distance, sound travels at 343m/sec, so to cm: 0.0343
  // Sound travels to the object and then back to the sensor, so we need to divide by 2
  // Is this accurate? no, but it's the best we have
  float distance = (timing * 0.0343) / 2;
  if (distance > 1500) return 0; // object blocking echo or trigger or both
  return distance;
}
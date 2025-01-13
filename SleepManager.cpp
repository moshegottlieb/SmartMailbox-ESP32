#ifdef ESP_PLATFORM
#include "SleepManager.h"
#include <Arduino.h>

class DeepSleepManager : public SleepManager {
public:
  DeepSleepManager(){}
protected:
  virtual void attemptSleep(){
    // not implemented yet
  }
};

class LightSleepManager : public SleepManager {
public:
  LightSleepManager(){}
protected:
  virtual void attemptSleep(){
    switch (esp_light_sleep_start()){
      case ESP_OK:
      Serial.println("Wakeup!");
      break;
    case ESP_ERR_SLEEP_TOO_SHORT_SLEEP_DURATION:
      Serial.println("Could not enter sleep: ESP_ERR_SLEEP_TOO_SHORT_SLEEP_DURATION");
      break;
    case ESP_ERR_SLEEP_REJECT:
      Serial.println("Could not enter sleep: ESP_ERR_SLEEP_REJECT");
      break;
    }
  }
};


SleepManager::SleepManager():_counter(0){
}
SleepManager& SleepManager::deep(){
    static DeepSleepManager sShared;
    return sShared;
}
SleepManager& SleepManager::light(){
    static LightSleepManager sShared;
    return sShared;
}


void SleepManager::sleep(){
    if (_counter > 0){
      --_counter;
    }
}

void SleepManager::wakeup(){
  ++_counter;
}

bool SleepManager::shouldSleep() const {
  return _counter == 0;
}

void SleepManager::commit(){
  if (shouldSleep()){
    Serial.println("Going to sleep");
    Serial.flush();
    attemptSleep();
  }
}

#endif // #ifdef ESP_PLATFORM
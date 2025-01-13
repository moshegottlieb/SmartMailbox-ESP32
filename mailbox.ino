#include "Peripheral.h"
#include "SleepManager.h"
#include "UltraSonicSensor.h"
#ifndef RGB_STATE_PERIPHERAL
#include "RGBLED.h"
#endif

Peripheral* pPeripheral = nullptr;
#ifndef RGB_STATE_PERIPHERAL
RGBLED* pRGB = nullptr;
#endif

// add as many as you want here.
// the more the merrier
// {echo pin, trigger pin}
uint8_t HCSR04_PAIRS[][2] = { {21,20}, {23,22} };
UltraSonicSensor** sensors = nullptr;
uint8_t sensor_count = (uint8_t) (sizeof(HCSR04_PAIRS) / (sizeof(uint8_t)*2));

void setup() {
  Serial.begin(115200);  // Initialize serial communication at 115200 baud
  Serial.println("Starting up");
  // Create an array of sensor pointers
  sensors = (UltraSonicSensor**)malloc(sizeof(UltraSonicSensor*) * sensor_count);
  // Initialize the array
  for (uint8_t i=0;i<sensor_count;++i){
    sensors[i] = new UltraSonicSensor(HCSR04_PAIRS[i][0],HCSR04_PAIRS[i][1]);
  }
  // the more the merrier
#ifndef RGB_STATE_PERIPHERAL
  pRGB = new RGBLED();
#endif
  esp_sleep_enable_timer_wakeup(3000000); // wake up every 3 seconds
}

bool hasLetter(){
  // You may want to adjust this to your mailbox.
  // Mine is 32.5cm high, I was going for 15 initially, but diagnoally placed letters thought otherwise.
  // Your mileage may vary.
  const float DISTANCE_THRESHOLD = 10.0;
  const float TIMEOUT_THRESHOLD = 1000.0;
  float distance;
  // Check sensors, stop when the first one crosses the thresholds
  for (uint8_t i=0;i<sensor_count;++i){
    distance = sensors[i]->read();
    if (distance > TIMEOUT_THRESHOLD || distance < DISTANCE_THRESHOLD){
     return true;
    }
  }
  // Thresholds within limits
  return false;
}

void loop() {
  Serial.println("Active");
  {
    // We need to dispose of the peripheral before going to sleep, otherwise sleep would fail
    // We actually need to call deinit, but there's absolutely no doc saying what happens if we 
    // leave the objects alive and just call init/deinit
    // could work, and could not, depending on the impl. which I didn't bother to check
    Peripheral peripheral;
    // Give the mobile device a 300ms window to connect, every time we wake up
    delay(300);
    // only utilize the ultra sonic sensor once, we want to save the battery
    bool did_check = false;    
    // Are we connected? stay alive while connected, go to sleep later
    while (peripheral.isConnected()){
      // Only check once per connection
      if (!did_check){
        // Is there a letter in the mailbox?
        bool has_letter = hasLetter();
        Serial.print("We ");
        if (!has_letter){ Serial.print("don't "); }
        Serial.println("have a letter.");
        // we can only count one item now, sometime in the future when I shape up my engineering skills
        // I will move to the reed switch solution that counts hatch open, and resets on door open.
        // I just couldn't connect the frigging magnets and make them stay
        peripheral.setCounter(has_letter?1:0);
        // don't check again
        did_check = true;
      }
      // stay alive but don't kill the cpu
      delay(100);
    }
  }
  // Peripheral is now dead, ok to sleep
  SleepManager::light().commit();
}

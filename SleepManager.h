#ifdef ESP_PLATFORM

#pragma once

#include <stdint.h>
/**
* @class SleepManager
* This is an ESP32 only class
* Manage light sleep reference count
* Increment counter by calling `wakeup`, decrement by calling `sleep`.
* Start sleep (if counter is zero) by calling `commit`.
* Counter is initialized to zero, so calling commit will initially make the system go to sleep
*/
class SleepManager {
protected:
  /**
  * Private, not intended for usage
  */
  SleepManager();
public:
  /**
   * @brief Shared light sleep manager
   * 
   * @return LightSleepManager& 
   */
  static SleepManager& light();
  /**
  * @brief shared deep sleep manager
  */
  static SleepManager& deep();
  /**
   * @brief No copy CTOR
   */
  SleepManager(SleepManager const&) = delete;
  /**
   * @brief No assignment operator
   */
  void operator=(SleepManager const&) = delete;
  /**
   * @brief Decrement sleep timer
   * Call this when you are done performing wake up code
   */
  void sleep();
  /**
   * @brief Wake up 
   * Call this when you start working on wakeup code, to increment the wakeup counter by one.
   * Do not forget to counter with a sleep call.
   */
  void wakeup();
  /**
   * @brief Query if the sleep manager will actually sleep on commit
   * 
   * @return true Will go to sleep
   * @return false Will not go to sleep
   */
  bool shouldSleep() const;
  /**
   * @brief Commit sleep mode
   * Try to go to sleep if the counter is zero
   */
  void commit();
protected:
  virtual void attemptSleep() = 0;
private:
  uint32_t _counter;
};

#endif //#ifdef ESP_PLATFORM
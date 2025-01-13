#pragma once

#include <BLEServer.h>
#include <BLECharacteristic.h>
#ifdef RGB_STATE_PERIPHERAL
#include "RGBLED.h"
#endif

class Peripheral : 
private BLEServerCallbacks,
private BLECharacteristicCallbacks {
public:
  Peripheral();
  virtual ~Peripheral();
protected:
  void startAdvertising();
  void stopAdvertising();
public:
  void setCounter(uint16_t counter);
  bool isConnected() const;
public:
  static const char* serviceUUID();
  static const char* countCharacteristicUUID();
  static const char* name();
private:
  virtual void onConnect(BLEServer *pServer);
  virtual void onDisconnect(BLEServer *pServer);
  virtual void onWrite(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t *param);
private:
  BLEServer* _server;
  BLEService* _service;
  BLECharacteristic* _counter;
  bool _connected;
#ifdef RGB_STATE_PERIPHERAL
  RGBLED _led;
#endif
};
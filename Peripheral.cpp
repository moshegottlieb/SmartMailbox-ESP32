#include "Peripheral.h"
#include "SleepManager.h"
#include <BLEDevice.h>
#include <BLE2902.h>

Peripheral::Peripheral()
:_connected(false){
  BLEDevice::init(Peripheral::name());
  // We only need BLE
  esp_bt_controller_enable(ESP_BT_MODE_BLE); // no need for BT classic 
  if (esp_sleep_enable_bt_wakeup() != ESP_OK){
    Serial.println("Failed to enable bt wakeup");
  }
  _server = BLEDevice::createServer();
  _server->setCallbacks(this);
  _service = _server->createService(BLEUUID(Peripheral::serviceUUID()));
  _counter = _service->createCharacteristic(BLEUUID(Peripheral::countCharacteristicUUID()),
      BLECharacteristic::PROPERTY_READ |
      BLECharacteristic::PROPERTY_WRITE |
      BLECharacteristic::PROPERTY_NOTIFY
  );
  _counter->setCallbacks(this);
  _counter->addDescriptor(new BLE2902());
  _service->addCharacteristic(_counter);
  BLEAdvertising* pAdvertising = _server->getAdvertising();
  pAdvertising->addServiceUUID(BLEUUID(Peripheral::serviceUUID()));
  pAdvertising->setAppearance((0x15/*Sensor*/ << 6) | 0x08/*Occupancy*/);
  // The following are in 1.25ms units, only the gods of BLE know why
  // These are the advertiesment intervals
  pAdvertising->setMaxInterval(80); // 100ms while active
  pAdvertising->setMinInterval(40); // 50ms while active
  // Once connected, intervals to exchange data in
  pAdvertising->setMaxPreferred(80); // 125ms
  pAdvertising->setMinPreferred(40); // 50ms
  _service->start();
  startAdvertising();
}

void Peripheral::setCounter(uint16_t counter){
  uint8_t data[2];
  data[0] = counter >> 8;
  data[1] = counter;
  _counter->setValue(data,2);
  if (_connected) 
    _counter->notify();
}



Peripheral::~Peripheral(){
  /*
  There's literally no documentation on how to release some of the classes here.
  From my tests, it works.
  This is of course the normal C++ way, however, since the server is created using BLEDevice::createServer() (or other xxx::createXXX),
  I expected it to have something like: BLEDevice::releaseServer(BLEServer*).
  */
  stopAdvertising();
  _server->removeService(_service);
  delete _service;
  _service = nullptr; // habbit
  delete _server;
  _server = nullptr;
  BLEDevice::deinit(false);
}

void Peripheral::startAdvertising(){
  BLEAdvertising* pAdvertising = _server->getAdvertising();
  pAdvertising->start();
#ifdef RGB_STATE_PERIPHERAL
  _led.set(0,0,31);
#endif
}
void Peripheral::stopAdvertising(){
  BLEAdvertising* pAdvertising = _server->getAdvertising();
  pAdvertising->stop();
#ifdef RGB_STATE_PERIPHERAL
  _led.set(0,0,0);
#endif
}

bool Peripheral::isConnected() const{
  return _connected;
}

void Peripheral::onConnect(BLEServer *pServer){
  Serial.println("Connected");
  _connected = true;
#ifdef RGB_STATE_PERIPHERAL
  _led.set(0,31,0);
#endif
  stopAdvertising();
  SleepManager::light().wakeup();
}
void Peripheral::onDisconnect(BLEServer *pServer){
  Serial.println("Disconnected");
  _connected = false;
  startAdvertising();
  SleepManager::light().sleep();
}

void Peripheral::onWrite(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t *param){
  if (pCharacteristic == _counter){
    uint8_t* data = pCharacteristic->getData();
    uint16_t res;
    if (pCharacteristic->getLength() == 2){
      res = data[1] | (data[0] << 8);
      Serial.println(res);
    } else {
      return; // error reading data
    }
#ifdef RGB_STATE_PERIPHERAL
    uint8_t pix = (res > 0) ? 15 : 0;
    _led.set(pix,pix,pix);
#endif
  }
}

const char* Peripheral::name() {
  return "Mailbox Service";
}

const char* Peripheral::serviceUUID(){
  return "ca7b329c-db95-4a4e-8903-19eabaa8c17a";
}
const char* Peripheral::countCharacteristicUUID(){
  return "44acd597-7535-4dc2-b1e2-081657b5ae47";
}

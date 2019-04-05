/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <Wire.h>

XXX

BLECharacteristic *pCharacteristic;
bool deviceConnected = false; 
int txValue = 0; 


// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID_TX "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyServerCallbacks: public BLEServerCallbacks
{
  void onConnect(BLEServer* pServer)
  {
    deviceConnected = true; 
  };

    void onDisconnect(BLEServer* pServer)
  {
    deviceConnected = false; 
  }
};

int16_t AcX;
const int MPU=0x68; 


void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  // Create BLE Device 
  BLEDevice::init("MY ESP32");
  // Create BLE Server 
  BLEServer *pServer = BLEDevice::createServer();
  pServer -> setCallbacks(new MyServerCallbacks()); 

  //Create the BLE Service 
  BLEService *pService = pServer->createService(SERVICE_UUID);

  //Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_TX,
                                         BLECharacteristic::PROPERTY_NOTIFY
                                       );

  //BLE2902 needed to notify 
  pCharacteristic->addDescriptor(new BLE2902());

  //Start Service 
  pService->start();

  //Start advertising
  pServer->getAdvertising()->start();
  
 
}

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 12, true);
  
  AcX = Wire.read() << 8 | Wire.read();
  AcX=AcX/16;

if (deviceConnected)
{
  txValue= AcX; 

  //Conversion of txValue
  char txString[16];
  dtostrf(txValue, 1, 2, txString); 

  //Setting the value to the characteristico 
  pCharacteristic -> setValue(txString); 

  //Notifying the connected client 
  pCharacteristic->notify();
  Serial.println("Sent value:" + String(txString));
  delay(500);
}

}

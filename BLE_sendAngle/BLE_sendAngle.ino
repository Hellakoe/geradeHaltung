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

BLECharacteristic *pCharacteristic;
bool deviceConnected = false; 
int txValue = 0; 
const int MPU = 0x68;
boolean value = LOW;
unsigned long previousMillis = 0;
unsigned long interval = 1000;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ = 0;
float gz_null = 0;
float angle = 0; 
int16_t GyZold, GyZdiff = 0;
float t1, t2 = 0;
unsigned long prev;

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

   for (int i=0;i<500;i++) //mittelung von nullpunkt
    {
      while (GyZdiff > 1)
      {
      GyZold=GyZ;
      AcX = Wire.read() << 8 | Wire.read();
      AcY = Wire.read() << 8 | Wire.read();
      AcZ = Wire.read() << 8 | Wire.read();
      GyX = Wire.read() << 8 | Wire.read();
      GyY = Wire.read() << 8 | Wire.read();
      GyZ = Wire.read() << 8 | Wire.read();  
      GyZ = GyZ/131;
      GyZdiff= GyZ-GyZold;
      }
          
      gz_null = gz_null+GyZ;
    }
    gz_null = gz_null/500;
    prev = millis();
 
}

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 12, true);
  
GyZold=GyZ,
  t1=t2; 

  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
  t2  = millis();
  t2=t2/1000;
  
GyZ= GyZ-gz_null;
GyZ=GyZ/131;
GyZ=GyZ+1; 
//GyZ in Winkelgeschwindigkeit deg/sec 
 
//Winkel berechnen 
//angle=angle+GyZ*(t2-t1)*(GyZold+GyZ);
angle=angle+GyZ*(t2-t1);

if (deviceConnected)
{
  txValue= angle; 

  //Conversion of txValue
  char txString[16];
  dtostrf(txValue, 1, 2, txString); 

  //Setting the value to the characteristico 
  pCharacteristic -> setValue(txString); 

  //Notifying the connected client 
  pCharacteristic->notify();
  Serial.println("Sent value:" + String(txString));
  //delay(500);
}

}

/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/
//
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
int16_t AcX, AcY, AcZ, AcXold, AcYold, AcZold  = 0;
int16_t AcXdiff = 2;
int16_t AcYdiff;
int16_t AcZdiff;
int16_t GyX, GyY, GyZ;
float AcXCal, AcYCal, AcZCal = 0;
float AcX_null, AcY_null, AcZ_null;

float Bet_null;
float Bet_Ac;
float Skalar;
float angle = 0;

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
      Serial.println("SDevice connected!");
    };

    void onDisconnect(BLEServer* pServer)
    {
      deviceConnected = false;
    }
};


void setup() {
  Serial.begin(9600);
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

  for (int i = 0; i < 1000; i++)
  {

    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 12, true);

    AcXCal = Wire.read() << 8 | Wire.read();
    AcYCal = Wire.read() << 8 | Wire.read();
    AcZCal = Wire.read() << 8 | Wire.read();

    AcXCal = AcXCal / 16;
    AcYCal = AcYCal / 16;
    AcZCal = AcZCal / 16;

    AcX_null = AcXCal + AcX_null;
    AcY_null = AcYCal + AcY_null;
    AcZ_null = AcZCal + AcZ_null;

  }
  AcX_null = AcX_null / 1000;
  AcY_null = AcY_null / 1000;
  AcZ_null = AcZ_null / 1000;

  //Betrag von Ac_NULL
  Bet_null = sqrt(sq(AcX_null)  + sq(AcZ_null));
  
  prev = millis();

}

void loop() {
  
 Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 12, true);

  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
  AcX = AcX / 16;
  AcY = AcY / 16;
  AcZ = AcZ / 16;


  //Beträge ausrechen
  Bet_Ac = sqrt(sq(AcX) + sq(AcZ));

  //Skalarpordukt
  Skalar = AcX_null * AcX + AcZ_null * AcZ;

  angle = Skalar / (Bet_Ac * Bet_null);
  angle = acos(angle);
  angle = angle * (360 / (2 * PI));

  if (deviceConnected)
  {
    txValue = angle;

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

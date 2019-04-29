//https://www.hackster.io/Nicholas_N/how-to-use-the-accelerometer-gyroscope-gy-521-6dfc19

// Load Wi-Fi library
#include "BluetoothSerial.h"

#define timeSeconds 10

#include <Wire.h>
#include <math.h>
const int MPU = 0x68;
boolean value = LOW;
unsigned long previousMillis = 0;
unsigned long interval = 1000;
unsigned long before = 0;
unsigned long after = 0;

int16_t AcX, AcY, AcZ, AcXold, AcYold, AcZold  = 0;
int16_t AcXdiff = 2;
int16_t AcYdiff;
int16_t AcZdiff;
int16_t GyX, GyY, GyZ;
float AcXCal, AcYCal, AcZCal = 0;
float AcX_null, AcY_null, AcZ_null;

int txValue = 0;

int16_t AcX_one; 
int16_t AcY_one; 
int16_t AcZ_one;


float Bet_null;
float Bet_Ac;
float Skalar;
float angle = 0;

unsigned long prev;

BluetoothSerial SerialBT;

void setup() {

  SerialBT.begin("My ESP32");

  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission();


  for (int i = 0; i < 500; i++)
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
  AcX_null = AcX_null / 500;
  AcY_null = AcY_null / 500;
  AcZ_null = AcZ_null / 500;

  //Betrag von Ac_NULL
  Bet_null = sqrt(sq(AcX_null)  + sq(AcZ_null));
}
void loop() {
  
  before= millis(); 
  
  for (int i = 0; i < 10; i++)
  {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 12, true);

    AcX_one = Wire.read() << 8 | Wire.read();
    AcY_one = Wire.read() << 8 | Wire.read();
    AcZ_one = Wire.read() << 8 | Wire.read();
    GyX = Wire.read() << 8 | Wire.read();
    GyY = Wire.read() << 8 | Wire.read();
    GyZ = Wire.read() << 8 | Wire.read();
    AcX_one = AcX_one / 16;
    AcY_one = AcY_one / 16;
    AcZ_one = AcZ_one / 16;

    AcX = AcX + AcX_one;
    AcY = AcY + AcY_one;
    AcZ = AcZ + AcZ_one;
  }
  after= millis(); 
  

  AcX=AcX/10; 
  AcY=AcY/10; 
  AcZ=AcZ/10;


  //Beträge ausrechen
  Bet_Ac = sqrt(sq(AcX) + sq(AcZ));

  //Skalarpordukt
  Skalar = AcX_null * AcX + AcZ_null * AcZ;

  angle = Skalar / (Bet_Ac * Bet_null);
  angle = acos(angle);
  angle = angle * (360 / (2 * PI));


  txValue = (int)angle;
  //Conversion of txValue
  char txString[16];
  dtostrf(txValue, 1, 2, txString);

  if ((millis() - prev) > 100)
  {
    prev = millis();
    SerialBT.println(txString);
   
    /*Serial.print("Gyro: ");
      Serial.print("GyZ = "); Serial.print(GyZ);
      Serial.print(" | GyZold = "); Serial.print(GyZold);
      Serial.print(" | t2 = "); Serial.print(t2);
      Serial.print(" | t1 = "); Serial.print(t1);
      Serial.print(" | angle = "); Serial.println(AcX_null);*/
    /*Serial.print("gz_null "); Serial.print(gz_null);*/
    Serial.println(AcX);
    Serial.println(AcZ);
    Serial.println(Bet_Ac);
    Serial.println(AcXCal);
    Serial.println(AcZCal);
    Serial.println(AcX_null);
    Serial.println(AcZ_null);
    Serial.println(Bet_null);
    Serial.println(Skalar);
    Serial.println(angle);
    Serial.println("-----TIME---");
    Serial.println(before);
    Serial.println(after);
    Serial.println("--------");
  }
}

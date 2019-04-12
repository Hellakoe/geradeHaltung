//https://www.hackster.io/Nicholas_N/how-to-use-the-accelerometer-gyroscope-gy-521-6dfc19

// Load Wi-Fi library

#include <Wire.h>
#include <math.h>
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
int b = 0;

float Bet_null;
float Bet_Ac;
float Skalar;
float angle = 0;

float t1, t2 = 0;
unsigned long prev;

void setup() {

  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission();




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




  if ((millis() - prev) > 100)
  {
    prev = millis();

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
    Serial.println(AcX_null);
    Serial.println(AcZ_null);
    Serial.println(Bet_null);
    Serial.println(Skalar);
    Serial.println(angle);
    Serial.println("--------");
  }
}

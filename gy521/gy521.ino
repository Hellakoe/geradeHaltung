//https://www.hackster.io/Nicholas_N/how-to-use-the-accelerometer-gyroscope-gy-521-6dfc19

// Load Wi-Fi library

#include <Wire.h>
const int MPU = 0x68;
boolean value = LOW;
unsigned long previousMillis = 0;
unsigned long interval = 1000;
int16_t AcX, AcX1, AcY, AcZ, Tmp, GyX, GyY, GyZ = 0;
float gz_null = 0;
float angle = 0; 
int16_t GyZold, GyZdiff = 4;
float t1, t2 = 0;
unsigned long prev;

void setup() {

  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
 
    for (int i=0;i<500;i++) //mittelung von nullpunkt
    {
      while (GyZdiff > 1)
      {
      GyZold=GyZ;
      AcX1 = Wire.read() << 8 | Wire.read();
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

    if((millis() - prev)>100)
    {
    prev =millis();
    
      Serial.print("Gyro: ");
      Serial.print("AcX1 = "); Serial.print(AcX1);
      Serial.print(" | GyZold = "); Serial.print(GyZold);
      Serial.print(" | t2 = "); Serial.print(t2);
      Serial.print(" | t1 = "); Serial.print(t1);
      Serial.print(" | angle = "); Serial.println(angle);
      Serial.print("gz_null "); Serial.print(gz_null);
      Serial.println(" ");
  }
}

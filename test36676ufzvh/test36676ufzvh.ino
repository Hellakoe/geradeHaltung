//https://www.hackster.io/Nicholas_N/how-to-use-the-accelerometer-gyroscope-gy-521-6dfc19

// Load Wi-Fi library

#include <Wire.h>
#include <math.h>
const int MPU = 0x68;
boolean value = LOW;
unsigned long previousMillis = 0;
unsigned long interval = 1000;
int16_t AcX, AcY, AcZ, AcXold, AcYold, AcZold  = 0;
int16_t AcXdiff, AcYdiff, AcZdiff = 2;
int16_t GyX, GyY, GyZ; 
int16_t AcXCal =-1;
int16_t AcYCal, AcZCal = 0; 
float AcX_null = 0; 
float AcY_null = 0;
float  AcZ_null = 0;
int b=0; 

float Bet_null, Bet_Ac = 0;
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
  Wire.endTransmission(true);
  
    
}
void loop() {
  Serial.println(AcZCal);
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  
  Wire.requestFrom(MPU, 12, true);
       for (int i=0;i<10;i++) //mittelung von nullpunkt
    {
      AcXCal = Wire.read() << 8 | Wire.read();
      AcYCal = Wire.read() << 8 | Wire.read();
      AcZCal = Wire.read() << 8 | Wire.read();
      GyX = Wire.read() << 8 | Wire.read();
      GyY = Wire.read() << 8 | Wire.read();
      GyZ = Wire.read() << 8 | Wire.read();
          
      AcX_null = AcX_null+AcXCal;
      AcY_null = AcY_null+AcYCal;
      AcZ_null = AcZ_null+AcZCal;

      Serial.println(AcXCal); 
      
    }
    AcX_null = AcX_null/10;
    AcY_null = AcY_null/10;
    AcZ_null = AcZ_null/10;

    Serial.println(AcX_null); 
    Serial.println("---------"); 
    
    //Betrag von Ac_NULL
    Bet_null = ((AcX_null)*(AcX_null) + (AcY_null)*(AcY_null) + (AcZ_null)*(AcZ_null));
    
    

}

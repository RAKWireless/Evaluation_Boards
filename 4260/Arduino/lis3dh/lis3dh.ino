//#include "SparkFunLIS3DH.h" ////https://github.com/sparkfun/SparkFun_LIS3DH_Arduino_Library/blob/master/examples/MultiI2C/MultiI2C.ino
#include <Wire.h>
  
//LIS3DH SensorTwo( I2C_MODE, 0x18 );

//global variable define
TwoWire *wi = &Wire;

void setup() {
  // Setup usb 
  Serial.begin(115200);
  while(!Serial);

  wi->beginTransmission(0x18);
  wi->write(byte(0x20));
  wi->write(byte(0x57)); 
  wi->endTransmission();

  delay(5);
  Serial.println("enter !");
  wi->beginTransmission(0x18);
  wi->write(byte(0x23));
  wi->write(byte(0x08));  
  wi->endTransmission();
  delay(5);  
//  if( SensorTwo.begin() != 0 )
//  {
//    Serial.println("Problem starting the sensor at 0x18.");
//  }
//  else
//  {
//    Serial.println("Sensor at 0x18 started.");
//  }
//    Serial.println("enter !");
}

void loop() {

  acc_data_show();
  delay(1000);
}
void acc_data_show()
{
  byte acc_h;
  byte acc_l;
  int x = 0;
  int y = 0;
  int z = 0;
  float accx;
  float accy;
  float accz;
  wi->beginTransmission(0x18);
  wi->write(byte(0x28));
  wi->endTransmission();
  wi->requestFrom(0x18, 1);
  while(wi->available()){
    acc_l = wi->read();
  } 
  wi->beginTransmission(0x18);
  wi->write(byte(0x29));
  wi->endTransmission();
  wi->requestFrom(0x18, 1);
  while(wi->available()){
    acc_h = wi->read();
  }
  x = (acc_h << 8) | acc_l;
  if(x<0x8000){x=x;}else{x=x-0x10000;}
  accx = x*4000/65536.0;
  
  wi->beginTransmission(0x18);
  wi->write(byte(0x2a));
  wi->endTransmission();
  wi->requestFrom(0x18, 1);
  while(wi->available()){
    acc_l = wi->read();
  } 
  wi->beginTransmission(0x18);
  wi->write(byte(0x2b));
  wi->endTransmission();
  wi->requestFrom(0x18, 1);
  while(wi->available()){
    acc_h = wi->read();
  }
  y = (acc_h << 8) | acc_l;
  if(y<0x8000){y=y;}else{y=y-0x10000;}
  accy = y*4000/65536.0;
  
  wi->beginTransmission(0x18);
  wi->write(byte(0x2c));
  wi->endTransmission();
  wi->requestFrom(0x18, 1);
  while(wi->available()){
    acc_l = wi->read();
  } 
  wi->beginTransmission(0x18);
  wi->write(byte(0x2d));
  wi->endTransmission();
  wi->requestFrom(0x18, 1);
  while(wi->available()){
    acc_h = wi->read();
  }
  z = (acc_h << 8) | acc_l;
  if(z<0x8000){z=z;}else{z=z-0x10000;}
  accz = z*4000/65536.0;
  Serial.print("ACC(mg): x=");
  Serial.print(accx);
  Serial.print(" y=");
  Serial.print(accy);
  Serial.print(" z=");
  Serial.println(accz);  
}
//void lis3dh_get(){
//  // read the sensor value
//  uint8_t cnt = 0;
//
//  // Pressure
//
//
//  Serial.print(" X2 = ");
//  Serial.println(SensorTwo.readFloatAccelX(), 4);
//  Serial.print(" Y2 = ");
//  Serial.println(SensorTwo.readFloatAccelY(), 4);
//  Serial.print(" Z2 = ");
//  Serial.println(SensorTwo.readFloatAccelZ(), 4);
//}

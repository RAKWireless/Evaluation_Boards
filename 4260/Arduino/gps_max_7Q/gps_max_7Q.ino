//max-7Q is a gps sensor communicate with mcu via uart. Only suit for Slot A on Wisblock


void setup() {
  // Setup usb 
  Serial.begin(115200);
  while(!Serial);
  Serial.print("max 7q power up!");
  //enable gps and serial1 for gps
  pinMode(GPS_EN,OUTPUT); 
  digitalWrite(GPS_EN,0);
  delay(1000);
  digitalWrite(GPS_EN,1);
  delay(1000);
  Serial1.begin(9600);
  while(!Serial1);
  while(1){Serial1.print("max 7q power up!");delay(1000);}
}

void loop() {

  if(Serial1.available())
  {
    int c = Serial1.read();
    Serial.print(c);
  }
}

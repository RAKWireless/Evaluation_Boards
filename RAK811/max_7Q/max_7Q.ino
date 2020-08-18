//max-7Q is a gps sensor communicate with mcu via uart. Only suit for Slot A on Wisblock

HardwareSerial Serial3(GPS_UART_RX, GPS_UART_TX);
void setup() {
  // Setup usb 
  Serial.begin(115200);
  while(!Serial);
  Serial.println("max 7q power up!");
  //enable gps and serial1 for gps
  pinMode(GPS_POWER_ON_PIN, OUTPUT);
  digitalWrite(GPS_POWER_ON_PIN, HIGH);
  pinMode(PB2,OUTPUT); 
  digitalWrite(PB2,0);
  delay(1000);
  digitalWrite(PB2,1);
  delay(1000);
  Serial3.begin(9600);
  while(!Serial3);
  Serial.println("gps uart init ok!");
}

void loop() {
  while (Serial3.available() > 0){
    // get the byte data from the GPS
    byte gpsData = Serial3.read();
    Serial.write(gpsData);
  }
}

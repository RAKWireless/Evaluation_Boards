//The demo just supplys the uart way for communicating with RS485/RS232 peripheral. 
//User should define the data protocol waht you need

//serial3 is used for communicating, do not modify it
HardwareSerial Serial3(GPS_UART_RX, GPS_UART_TX);


void setup() {
  // Setup usb 
  Serial.begin(115200);
  while(!Serial);
  Serial.println("RS485 demo!");
  //RS485 uart init
  Serial3.begin(115200);
  while(!Serial3);
  Serial.println("RS485 uart init ok!");
}

void loop() {
    byte Data = 0xaa;
    //send 
    Serial3.write(Data);
    //recieve 
    while (Serial3.available() > 0){
    Data = Serial3.read();
    Serial.write(Data);
  }
}

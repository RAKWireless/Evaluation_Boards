
#define BG77_POWER_KEY    17
String bg77_rsp = "";
void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb
  Serial.println("RAK4630 Cellular TEST!");
 
  //BG77 init
  pinMode(BG77_POWER_KEY, OUTPUT);
  digitalWrite(BG77_POWER_KEY,0);
  delay(1000);
  digitalWrite(BG77_POWER_KEY,1);
  delay(2000);
  digitalWrite(BG77_POWER_KEY,0);
  Serial1.begin(115200);
  delay(1000);
  //while(!mySerial);
  Serial.println("BG77 power up!");
}
//this function is suitable for most AT commands of bg96. e.g. bg96_at("ATI")
void bg77_at(char *at)
{
  char tmp[256] = {0};
  int len = strlen(at);
  strncpy(tmp,at,len);
  tmp[len]='\r';
  Serial1.write(tmp);
  delay(10);
  while(Serial1.available()){
      bg77_rsp += char(Serial1.read());
      delay(2);
  }
  Serial.println(bg77_rsp);
  bg77_rsp="";
}
void loop() // run over and over//
{ 
    bg77_at("ATI");
    delay(2000);
//  if (mySerial.available()) 
//    Serial.write(mySerial.read());
//
//  if (Serial.available())
//    mySerial.write(Serial.read());
}

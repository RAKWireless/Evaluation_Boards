/*
 * This sketch demonstrate how to communication with esp32-at module with RAK4630.
 *
 * RAK4630 <---AT---> esp32-at module
 *
 */
void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb
  Serial.println("RAK4630 ESP32 AT CMD TEST!");

  //esp32 init
  Serial1.begin(115200);
  delay(1000);
  // The ESP32 AT commands refer to https://github.com/espressif/esp-at/blob/master/docs/ESP_AT_Commands_Set.md
  // esp32_at("AT+SYSSTORE=1",500);
  esp32_at("AT+CWCOUNTRY?",500);
  // esp32_at("AT+CWCOUNTRY=1,\"CN\",1,13",500);
  Serial.println("Search the Wi-Fi Mode:");
  esp32_at("AT+CWMODE?",500);
  Serial.println("Sets the Wi-Fi Mode Station+SoftAP:");
  esp32_at("AT+CWMODE=3",500);
  Serial.println("The ESP32 SoftAP:");
  esp32_at("AT+CWSAP?",500);
  Serial.println("Lists the Available APs:");
  esp32_at("AT+CWLAPOPT=1,31",500);
  esp32_at("AT+CWLAP",500);
}

//this function is suitable for most AT commands of esp32. e.g. esp32_at("AT")
void esp32_at(char *at, uint16_t timeout)
{
  String esp32_rsp = "";
  char tmp[128] = {0};
  int len = strlen(at);
  strncpy(tmp, at, len);
  tmp[len]='\r';
  tmp[len+1]='\n';
  Serial1.write(tmp);
  delay(10);
  while (timeout--)
  {
    if(Serial1.available()>0)
    {
      esp32_rsp += char(Serial1.read());
    }
    delay(1);
  }
  Serial.print(esp32_rsp);
}

void loop()
{
  int timeout = 100;
  String resp = "";
  while(timeout--)
  {
    if(Serial1.available()>0)
    {
      resp += char(Serial1.read());
    }
    delay(1);
  }
  if(resp.length() > 0)
  {
    Serial.print(resp);
  }
  resp = "";
}

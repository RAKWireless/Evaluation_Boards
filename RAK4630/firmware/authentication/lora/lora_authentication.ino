#include "radio.h"
#include "sx126x-board.h"
#include "sx126x.h"
#include <SPI.h>

#define LED 35
//#define Receive

#define CTR1  37
#define CTR2  39

String parse_str="";
uint32_t fre[72];
uint8_t  fix_flag = 0;

#define USE_MODEM_LORA

#define RF_FREQUENCY                                868300000//868300000 //928000000 //868300000 // Hz

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7        // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_SYMBOL_TIMEOUT                         5         // Symbols
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false     // false
#define LORA_IQ_INVERSION_ON                        false     //  false

#define TX_OUTPUT_POWER                             22
static RadioEvents_t RadioEvents;
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );
void OnTxDone(void);
void OnTxTimeout(void);
String rsp = "";
// Visit your thethingsnetwork.org device console
// to create an account, or if you need your session keys.

// Network Session Key (MSB)
uint8_t NwkSkey[16] = { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };

// Application Session Key (MSB)
uint8_t AppSkey[16] = { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };

// Device Address (MSB)
uint8_t DevAddr[4] = { 0x11, 0x11, 0x11, 0x11 };

uint8_t packet[] = {'1','2','3','4','5'};

 RadioStatus_t status;
  uint16_t Interrupt;
  RadioError_t error;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT);
  pinMode(37,OUTPUT);
  pinMode(39,OUTPUT);
  digitalWrite(37,LOW);
  digitalWrite(39,HIGH);
  SPI.begin();
  SX126xIoInit();
  // put your setup code here, to run once:
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb
  Serial.println("******************************Arduino on Rak4630******************************");
  int j = 0;
  fre[0] = 902300000;
  //生成跳频数组
  for(j;j<72;j++)
  {
    fre[j] = fre[0] + j*200000;
  }
  fre[64]=903000000;
  fre[65]=904600000;  
  fre[66]=906200000;
  fre[67]=907800000; 
  fre[68]=909400000;
  fre[69]=911000000; 
  fre[70]=912600000;
  fre[71]=914200000;  

  randomlize(fre,72);
//    for(j=0;j<72;j++)
//  {
//    Serial.println(fre[j]);
//  }
  //SX126xCalibrateImage(RF_FREQUENCY);
  RadioEvents.RxDone = NULL;//OnRxDone;
  RadioEvents.TxDone = NULL;//OnTxDone;
  RadioEvents.TxTimeout = NULL;//OnTxTimeout;

  Radio.Init( &RadioEvents );
  Serial.println("Init events");

//  Radio.SetChannel( RF_FREQUENCY );
//  Serial.println("Set rf frequency");
//
//  Radio.SetPublicNetwork(true);
#ifdef Receive
  Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                     LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                     LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                     0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
  Serial.println("Set Rx config");
#else
//  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
//                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
//                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
//                                   true, 0, 0, LORA_IQ_INVERSION_ON, 0 );
//  Serial.println("Set Tx config");
  //定频
  
  while(1)
  {
      if(Serial.available())
      {
         rsp += char(Serial.read());
      }
      if(rsp.indexOf("at+begin=lorap2p")!= -1)
      {
         
         if(rsp.indexOf(":F")!= -1)
         {
            
            fix_flag = 1;
            parse_str="";
            parse_str=rsp;
            rsp = "";
         }
      }
      if(fix_flag == 1)
      {
        Serial.println("lora fix begin!!");
        fix_send();
        fix_flag = 0;
      }
      if(rsp.indexOf("at+end=lorap2p")!= -1)
      {
         Serial.println("lora fix end!!");
         break;
      }
      //delay(1);
  }
  rsp = "";
  //跳频  
  while(1)
  {
      if(Serial.available())
      {
         rsp += char(Serial.read());
      }
      if(rsp.indexOf("at+begin=lorahop")!= -1)
      {
         Serial.println("lora hop test!!");
         break;
      }
    //delay(1);
  }  
#endif
  int k = 0;
  while(1)
  {
   k = 0;
  for(k;k<72;k++)
  {
    //Serial.print("Send in ");
    Serial.println(fre[k]);
    Radio.SetChannel( fre[k] );
    Radio.SetPublicNetwork(true);
    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 0 );
    Radio.Send(packet,5); 
    delay(150);
  }
   Serial.print("new send again!!");
  }
}


void loop() {
  
  Serial.println("Finish");
  delay(2000);
}

void randomlize(uint32_t *a, int n)
{
    int i = 0,j = 0, k = 0;    
    for(i = 0; i < n; i++)        
    {        
        j = random(50)%(n-i)+i;        
        k = a[i];        
        a[i] = a[j];        
        a[j] = k;        
    }    
}
void fix_send()
{
    int index = parse_str.indexOf(":");
    int i = 0;
    uint32_t fre = 0;
    char tmp[10] = {0};
    index++;
    for(i;parse_str[index]!=':';i++)
    {
      tmp[i] = parse_str[index++];
    }
    fre = atoi(tmp);
    Serial.print("frequency:");
    Serial.println(fre);
    index++;
    uint32_t band = 0;
    band = parse_str[index] - '0';
    Serial.print("bandwidth: (0:  125KHz     1:  250KHz      2:  500KHz):");
    Serial.println(band);
    index++;
    index++;     
    uint8_t code = 0; 
    code = parse_str[index] - '0';
    Serial.print("codeingrate: (1:  4/5  2:  4/6  3:  4/7   4:  4/8)  :");
    Serial.println(code);   
    index++;
    index++;
    i = 0;
    memset(tmp,0,10);
    for(i;parse_str[index]!=':';i++)
    {
      tmp[i] = parse_str[index++];
    }
    uint32_t datarate = 0;
    datarate = atoi(tmp);
    Serial.print("datarate: (6: 64  7: 128  8: 256  9: 512  10: 1024  11: 2048  12: 4096): ");
    Serial.println(datarate);
    index++;
    i = 0;
    memset(tmp,0,10);
    for(i;parse_str[index]!=':';i++)
    {
      tmp[i] = parse_str[index++];
    }       
    int8_t power = 0;
    power = atoi(tmp);
    Serial.print("power:(TX power. The unit is dbm. -3~22):    ");
    Serial.println(power);  
        
    Radio.SetChannel( fre );
    Radio.SetPublicNetwork(true);
    Radio.SetTxConfig( MODEM_LORA, power, 0, band,datarate, code,LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,true, 0, 0, LORA_IQ_INVERSION_ON, 0);
    Radio.Send(packet,5); 
}
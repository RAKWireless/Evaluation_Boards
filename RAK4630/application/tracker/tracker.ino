#include <Arduino.h>
#include <LoRaWan-Arduino.h>
#include <SPI.h>
#include "SparkFunLIS3DH.h" ////https://github.com/sparkfun/SparkFun_LIS3DH_Arduino_Library/blob/master/examples/MultiI2C/MultiI2C.ino
#include "Wire.h"
#include <SoftwareSerial.h>


SoftwareSerial Serial3(15, 16);  //GPS_UART_RX,GPS_UART_TX
LIS3DH SensorTwo( I2C_MODE, 0x18 );

// RAK4630 hardware pin configuration, do not modify here
int PIN_LORA_RESET = 38;  // LORA RESET
int PIN_LORA_NSS = 42;   // LORA SPI CS
int PIN_LORA_SCLK = 43;  // LORA SPI CLK
int PIN_LORA_MISO = 45;  // LORA SPI MISO
int PIN_LORA_DIO_1 = 47; // LORA DIO_1
int PIN_LORA_BUSY = 46;  // LORA SPI BUSY
int PIN_LORA_MOSI = 44;  // LORA SPI MOSI
int RADIO_TXEN = 37;	 // LORA ANTENNA TX ENABLE
int RADIO_RXEN = 39;	 // LORA ANTENNA RX ENABLE
// Replace PIN_SPI_MISO, PIN_SPI_SCK, PIN_SPI_MOSI with your
SPIClass SPI_LORA(NRF_SPIM2, PIN_LORA_MISO, PIN_LORA_SCLK, PIN_LORA_MOSI);

// RAK4630 supply two LED
#ifndef LED_BUILTIN
#define LED_BUILTIN 35
#endif

#ifndef LED_BUILTIN2
#define LED_BUILTIN2 36
#endif

//lorawan parameters, user can modify. Join type(OTAA/ABP)/ Dutycycle / Region / network are in Commissioning.h
#define SCHED_MAX_EVENT_DATA_SIZE APP_TIMER_SCHED_EVENT_DATA_SIZE /**< Maximum size of scheduler events. */
#define SCHED_QUEUE_SIZE 60  /**< Maximum number of events in the scheduler queue. */
#define LORAWAN_ADR 1 /**< LoRaWAN Adaptive Data Rate enabled (the end-device should be static here). */
#define LORAWAN_DATERATE DR_0 /*LoRaMac datarates definition, from DR_0 to DR_5*/
#define LORAWAN_TX_POWER TX_POWER_5 /*LoRaMac tx power definition, from TX_POWER_0 to TX_POWER_15*/
#define JOINREQ_NBTRIALS 3 /**< Number of trials for the join request. */
DeviceClass_t gCurrentClass = CLASS_A; /* class definition*/
lmh_confirm gCurrentConfirm = LMH_CONFIRMED_MSG; /* confirm/unconfirm packet definition*/
uint8_t gAppPort = LORAWAN_APP_PORT;   /* data port*/

/**@brief Structure containing LoRaWan parameters, needed for lmh_init()
 */
static lmh_param_t lora_param_init = {LORAWAN_ADR, LORAWAN_DATERATE, LORAWAN_PUBLIC_NETWORK, JOINREQ_NBTRIALS, LORAWAN_TX_POWER};


// Foward declaration
static void lorawan_has_joined_handler(void);
static void lorawan_rx_handler(lmh_app_data_t *app_data);
static void lorawan_confirm_class_handler(DeviceClass_t Class);
static void send_lora_frame(void);

/**@brief Structure containing LoRaWan callback functions, needed for lmh_init()
*/
static lmh_callback_t lora_callbacks = {BoardGetBatteryLevel, BoardGetUniqueId, BoardGetRandomSeed,
										lorawan_rx_handler, lorawan_has_joined_handler, lorawan_confirm_class_handler};


//OTAA keys
uint8_t nodeDeviceEUI[8] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x22, 0x22};
uint8_t nodeAppEUI[8] = {0xB8, 0x27, 0xEB, 0xFF, 0xFE, 0x39, 0x00, 0x00};
uint8_t nodeAppKey[16] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,0x88, 0x88, 0x88, 0x88, 0x22, 0x22, 0x22, 0x22};

//ABP keys
uint32_t nodeDevAddr = 0x11111111;
uint8_t nodeNwsKey[16] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
uint8_t nodeAppsKey[16] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};

#ifdef NRF52_SERIES
// Start BLE if we compile for nRF52
#include <bluefruit.h>
void initBLE();
extern bool bleUARTisConnected;
extern BLEUart bleuart;
#endif

// Private defination
#define LORAWAN_APP_DATA_BUFF_SIZE 64  /**< Size of the data to be transmitted. */
#define LORAWAN_APP_TX_DUTYCYCLE 2000 /**< Defines the application data transmission duty cycle. 20s, value in [ms]. */
static uint8_t m_lora_app_data_buffer[LORAWAN_APP_DATA_BUFF_SIZE]; //< Lora user application data buffer.
static lmh_app_data_t m_lora_app_data = {m_lora_app_data_buffer, 0, 0, 0, 0}; //< Lora user application data structure.

TimerEvent_t appTimer;
static uint32_t timers_init(void);
static uint32_t count = 0;
static uint32_t count_fail = 0;

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

    hw_config hwConfig;
	// Define the HW configuration between MCU and SX126x
	hwConfig.CHIP_TYPE = SX1262_CHIP;		  // Example uses an eByte E22 module with an SX1262
	hwConfig.PIN_LORA_RESET = PIN_LORA_RESET; // LORA RESET
	hwConfig.PIN_LORA_NSS = PIN_LORA_NSS;	 // LORA SPI CS
	hwConfig.PIN_LORA_SCLK = PIN_LORA_SCLK;   // LORA SPI CLK
	hwConfig.PIN_LORA_MISO = PIN_LORA_MISO;   // LORA SPI MISO
	hwConfig.PIN_LORA_DIO_1 = PIN_LORA_DIO_1; // LORA DIO_1
	hwConfig.PIN_LORA_BUSY = PIN_LORA_BUSY;   // LORA SPI BUSY
	hwConfig.PIN_LORA_MOSI = PIN_LORA_MOSI;   // LORA SPI MOSI
	hwConfig.RADIO_TXEN = RADIO_TXEN;		  // LORA ANTENNA TX ENABLE
	hwConfig.RADIO_RXEN = RADIO_RXEN;		  // LORA ANTENNA RX ENABLE
	hwConfig.USE_DIO2_ANT_SWITCH = false;	  // Example uses an CircuitRocks Alora RFM1262 which uses DIO2 pins as antenna control
	hwConfig.USE_DIO3_TCXO = true;			  // Example uses an CircuitRocks Alora RFM1262 which uses DIO3 to control oscillator voltage
	hwConfig.USE_DIO3_ANT_SWITCH = false;	 // Only Insight ISP4520 module uses DIO3 as antenna control

	// Initialize Serial for debug output
	Serial.begin(115200);
  while(!Serial){delay(10);}
	Serial.println("=====================================");
	Serial.println("Welcome to RAK4630 LoRaWan!!!");
#if (OVER_THE_AIR_ACTIVATION != 0)
  Serial.println("Type: OTAA");
#else
  Serial.println("Type: ABP");
#endif

#if defined(REGION_AS923)
    Serial.println("Region: AS923");
#elif defined(REGION_AU915)
    Serial.println("Region: AU915");
#elif defined(REGION_CN470)
    Serial.println("Region: CN470");
#elif defined(REGION_CN779)
    Serial.println("Region: CN779");
#elif defined(REGION_EU433)
    Serial.println("Region: EU433");
#elif defined(REGION_IN865)
    Serial.println("Region: IN865");
#elif defined(REGION_EU868)
    Serial.println("Region: EU868");
#elif defined(REGION_KR920)
    Serial.println("Region: KR920");
#elif defined(REGION_US915)
    Serial.println("Region: US915");
#elif defined(REGION_US915_HYBRID)
    Serial.println("Region: US915_HYBRID");
#else
    Serial.println("Please define a region in the compiler options.");
#endif
    Serial.println("=====================================");

  //lis3dh init 
  if( SensorTwo.begin() != 0 )
  {
    Serial.println("Problem starting the sensor at 0x18.");
  }
  else
  {
    Serial.println("Sensor at 0x18 started.");
  }
  //gps init 
  pinMode(17, OUTPUT);
  digitalWrite(17, HIGH);
  pinMode(34,OUTPUT); 
  digitalWrite(34,0);
  delay(1000);
  digitalWrite(34,1);
  delay(1000);
  Serial3.begin(9600);
  while(!Serial3);
  Serial.println("gps uart init ok!");

	// Initialize Scheduler and timer
    uint32_t err_code;

  err_code = timers_init();
	if (err_code != 0)
	{
		Serial.printf("timers_init failed - %d\n", err_code);
	}

	// Initialize LoRa chip.
	lora_hardware_init(hwConfig);

	// Setup the EUIs and Keys
#if (OVER_THE_AIR_ACTIVATION != 0)
	lmh_setDevEui(nodeDeviceEUI);
	lmh_setAppEui(nodeAppEUI);
	lmh_setAppKey(nodeAppKey);
#else
	lmh_setNwkSKey(nodeNwsKey);
	lmh_setAppSKey(nodeAppsKey);
	lmh_setDevAddr(nodeDevAddr);
#endif

	// Initialize LoRaWan
	err_code = lmh_init(&lora_callbacks, lora_param_init);
	if (err_code != 0)
	{
		Serial.printf("lmh_init failed - %d\n", err_code);
	}

	// EU868 KR920 IN865 AS923 set to 1
  // US915 AU915 set to 2
	if (!lmh_setSubBandChannels(1))
	{
		Serial.println("lmh_setSubBandChannels failed. Wrong sub band requested?");
	}

	// Start Join procedure
	lmh_join();
}

void loop()
{
	 // Handle Radio events
	 Radio.IrqProcess();
}

/**@brief LoRa function for handling HasJoined event.
 */
void lorawan_has_joined_handler(void)
{
#if (OVER_THE_AIR_ACTIVATION != 0)
	Serial.println("OTAA Mode, Network Joined!");
#else
	Serial.println("ABP Mode!");
#endif

  lmh_error_status ret = lmh_class_request(gCurrentClass);
  if(ret == LMH_SUCCESS)
  {
    //delay(1000);
  	TimerSetValue(&appTimer, LORAWAN_APP_TX_DUTYCYCLE);
  	TimerStart(&appTimer);
  }
}

/**@brief Function for handling LoRaWan received data from Gateway
 *
 * @param[in] app_data  Pointer to rx data
 */
void lorawan_rx_handler(lmh_app_data_t *app_data)
{
	Serial.printf("LoRa Packet received on port %d, size:%d, rssi:%d, snr:%d, data:%s\n",
				  app_data->port, app_data->buffsize, app_data->rssi, app_data->snr, app_data->buffer);

	switch (app_data->port)
	{
#if 0
	case 3:
		// Port 3 switches the class
		if (app_data->buffsize == 1)
		{
            if (app_data->buffer[0] == CLASS_A
                || app_data->buffer[0] == CLASS_B
                || app_data->buffer[0] == CLASS_C)
            {
                if(gCurrentClass != app_data->buffer[0])
                {
                    if(LMH_SUCCESS == lmh_class_request((DeviceClass_t)(app_data->buffer[0])))
                    {
                        gCurrentClass = (DeviceClass_t)(app_data->buffer[0]);
                    }
                }
            }
		}
		break;
	case LORAWAN_APP_PORT:
		// YOUR_JOB: Take action on received data
		break;
#endif
	default:
		break;
	}
}

void lorawan_confirm_class_handler(DeviceClass_t Class)
{
    Serial.printf("switch to class %c done\n", "ABC"[Class]);
    // Informs the server that switch has occurred ASAP
    m_lora_app_data.buffsize = 0;
    m_lora_app_data.port = gAppPort;
    lmh_send(&m_lora_app_data, gCurrentConfirm);
}

void send_lora_frame(void)
{
	if (lmh_join_status_get() != LMH_SET)
	{
		//Not joined, try again later
		// Serial.println("Did not join network, skip sending frame");
		return;
	}


    lmh_error_status error = lmh_send(&m_lora_app_data, gCurrentConfirm);
    if (error == LMH_SUCCESS)
    {
        count++;
        Serial.printf("lmh_send ok count %d\n", count);
    }
    else
    {
        count_fail++;
        Serial.printf("lmh_send fail count %d\n", count_fail);
    }
    TimerSetValue(&appTimer, LORAWAN_APP_TX_DUTYCYCLE);
    TimerStart(&appTimer);
}

String comdata = "";
/* return 0 ok, 1 fail*/
/*$GPRMC,080655.00,A,4546.40891,N,12639.65641,E,1.045,328.42,170809,,,A*60*/
int parse_gps()
{
  if(comdata.indexOf(",V,")!= -1)
    return 1;

  return 0;
}
/**@brief Function for handling a LoRa tx timer timeout event.
 */
void tx_lora_periodic_handler(void)
{
   uint32_t i = 0;
   uint32_t j = 0;
   uint32_t k = 0;
   int res = 1;
   float x = 0;
   float y = 0;
   float z = 0;
   float z1 = 0;
   Serial.println("check acc!");
   x = SensorTwo.readFloatAccelX();
   y = SensorTwo.readFloatAccelY();
   z = SensorTwo.readFloatAccelZ();
   Serial.print(" X = ");
   Serial.println(x, 4);
   Serial.print(" Y = ");
   Serial.println(y, 4);
   Serial.print(" Z = ");
   Serial.println(z, 4);
   if( abs(x-z) < 0.4)
   {
       while (Serial3.available() > 0){
       // get the byte data from the GPS
          comdata += char(Serial3.read());
          delay(2);
          if(comdata.indexOf("GPRMC")!= -1 && comdata.indexOf("GPVTG")!= -1)
            {
               break;
            }
       }
	   //for test
       //comdata = "$GPRMC,080655.00,A,4546.40891,N,12639.65641,E,1.045,328.42,170809,,,A*60";
       Serial.println(comdata);
       res = parse_gps();
       if(res == 1)
       {
            TimerSetValue(&appTimer, LORAWAN_APP_TX_DUTYCYCLE);
            TimerStart(&appTimer);
            return;
       }
       delay(1000);
       j = comdata.indexOf(",A,");
       j = j+3;
       if(comdata.indexOf(",E,") != -1)
       {
          k = comdata.indexOf(",E,");
          k = k+1;
       }
       else
       {
          k = comdata.indexOf(",W,");
          k = k+1;
       }

       memset(m_lora_app_data.buffer, 0, LORAWAN_APP_DATA_BUFF_SIZE);
       m_lora_app_data.port = gAppPort;
       m_lora_app_data.buffer[i++] = 0x09;  
       m_lora_app_data.buffer[i++] = ','; 	   
       while(j!=k+1)
       {
         m_lora_app_data.buffer[i++] = comdata[j];
         j++;
       }
       m_lora_app_data.buffsize = i;
       comdata = "";
       
       send_lora_frame();
   }
   else
   {
      TimerSetValue(&appTimer, LORAWAN_APP_TX_DUTYCYCLE);
      TimerStart(&appTimer);
   }
}

/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module. This creates and starts application timers.
 */
uint32_t timers_init(void)
{
    TimerInit(&appTimer, tx_lora_periodic_handler);
    return 0;
}

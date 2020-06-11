#include <Arduino.h>

#include <LoRaWan-Arduino.h>
#include <SX126x-Arduino.h>
#include <SPI.h>

#define ULB6_SENSOR

// Hardware pin
#ifdef NRF52_SERIES
// nRF52832 - SX126x pin configuration
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

// Check if the board has an LED port defined
#ifndef LED_BUILTIN
#define LED_BUILTIN 35
#endif

#endif

#define SCHED_MAX_EVENT_DATA_SIZE APP_TIMER_SCHED_EVENT_DATA_SIZE /**< Maximum size of scheduler events. */
#define SCHED_QUEUE_SIZE 60  /**< Maximum number of events in the scheduler queue. */

#define LORAWAN_ADR 1 /**< LoRaWAN Adaptive Data Rate enabled (the end-device should be static here). */
#define LORAWAN_DATERATE DR_0
#define LORAWAN_TX_POWER TX_POWER_0
#define JOINREQ_NBTRIALS 3 /**< Number of trials for the join request. */

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

DeviceClass_t gCurrentClass = CLASS_A;
//DeviceClass_t gCurrentClass = CLASS_C;
lmh_confirm gCurrentConfirm = LMH_CONFIRMED_MSG;
//lmh_confirm gCurrentConfirm = LMH_UNCONFIRMED_MSG;
uint8_t gAppPort = LORAWAN_APP_PORT;
//uint8_t nodeDeviceEUI[8] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x11, 0x11};
//uint8_t nodeAppEUI[8] = {0xB8, 0x27, 0xEB, 0xFF, 0xFE, 0x39, 0x00, 0x00};
//uint8_t nodeAppKey[16] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x11, 0x11};

uint8_t nodeDeviceEUI[8] = {0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77};
uint8_t nodeAppEUI[8] = {0xB8, 0x27, 0xEB, 0xFF, 0xFE, 0x39, 0x00, 0x00};
uint8_t nodeAppKey[16] = {0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77,0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77};

/**
 * ABP CONFIG
 */
uint32_t nodeDevAddr = 0x99999999;
uint8_t nodeNwsKey[16] = {0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99};
uint8_t nodeAppsKey[16] = {0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99};
/*
 * ABP CONFIG
 */

 
#ifdef NRF52_SERIES
// Start BLE if we compile for nRF52
//#include <bluefruit.h>
//void initBLE();
//extern bool bleUARTisConnected;
//extern BLEUart bleuart;
#endif

// Private defination
#define LORAWAN_APP_DATA_BUFF_SIZE 64  /**< Size of the data to be transmitted. */
#define LORAWAN_APP_TX_DUTYCYCLE 20000 /**< Defines the application data transmission duty cycle. 20s, value in [ms]. */
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

 /*
  * WisBLOCK 5811 Power On
  */
  pinMode(17, OUTPUT);
  digitalWrite(17, HIGH);
  //analogReference();

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
	Serial.println("SX126x LoRaWan test");
    Serial.println("=====================================");

// #ifdef NRF52_SERIES
// 	pinMode(30, OUTPUT);
// 	digitalWrite(30, HIGH);
// 	// Start BLE if we compile for nRF52
// 	initBLE();
// #endif

	// Initialize Scheduler and timer
    uint32_t err_code;

    err_code = timers_init();
	if (err_code != 0)
	{
		Serial.printf("timers_init failed - %d\n", err_code);
	}

	// Initialize LoRa chip.
	err_code = lora_hardware_init(hwConfig);
	if (err_code != 0)
	{
		Serial.printf("lora_hardware_init failed - %d\n", err_code);
	}

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

	// For some regions we might need to define the sub band the gateway is listening to
	// This must be called AFTER lmh_init()
	/// \todo This is for Dragino LPS8 gateway. How about other gateways???
//#if #define REGION_AU915
//  if (!lmh_setSubBandChannels(2))
//#else
  if (!lmh_setSubBandChannels(1))
//#endif
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

	// We are on FreeRTOS, give other tasks a chance to run
	// delay(100);
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

	lmh_class_request(gCurrentClass);

	TimerSetValue(&appTimer, LORAWAN_APP_TX_DUTYCYCLE);
	TimerStart(&appTimer);
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

#ifdef ULB6_SENSOR

int get_depths(void)
{
  int i;
  
  int sensor_pin = A1;   // select the input pin for the potentiometer
  int mcu_ain_value = 0;  // variable to store the value coming from the sensor

  int depths;         // variable to store the value of oil depths
  int average_value;  
  float voltage_ain, voltage_sensor;

  for (i = 0; i < 5; i++)
  {
    mcu_ain_value += analogRead(sensor_pin);
  }
  average_value = mcu_ain_value / i;
  
  voltage_ain = average_value * 3.6 /1024;  //raef 3.6v / 10bit ADC 
  
  voltage_sensor = voltage_ain / 0.6;       //WisBlock RAK5811 (0 ~ 5V).   Input signal reduced to 6/10 and output
  
  depths =  (voltage_sensor * 1000 - 574) * 2.5; //Convert to millivolt. 574mv is the default output from sensor
  
  Serial.printf("-------average_value------ = %d\n", average_value);
  Serial.printf("-------voltage_sensor------ = %f\n", voltage_sensor);
  Serial.printf("-------depths------ = %d mm\n", depths);
}
#endif

void send_lora_frame(void)
{
  int depths;
  
	if (lmh_join_status_get() != LMH_SET)
	{
		//Not joined, try again later
		// Serial.println("Did not join network, skip sending frame");
		return;
	}

  depths = get_depths();      //Depth range: (0 ~ 5000mm)
  
	uint32_t i = 0;

	m_lora_app_data.port = gAppPort;
	m_lora_app_data.buffer[i++] = 0x07;
	m_lora_app_data.buffer[i++] = (depths >> 8) && 0xFF;
	m_lora_app_data.buffer[i++] = depths && 0xFF;
	m_lora_app_data.buffsize = i;

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
}

/**@brief Function for handling a LoRa tx timer timeout event.
 */
void tx_lora_periodic_handler(void)
{
   TimerSetValue(&appTimer, LORAWAN_APP_TX_DUTYCYCLE);
   TimerStart(&appTimer);
   Serial.println("Sending frame now...");
   send_lora_frame();
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

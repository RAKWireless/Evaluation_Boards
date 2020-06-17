#include <Arduino.h>

#include <SX126x-Arduino.h>
#include <SPI.h>
#include <bluefruit.h>
#define ADV_TIMEOUT   600 // seconds

hw_config hwConfig;

// RAK4630 hardware pin configuration, do not modify here
int PIN_LORA_RESET = 38;  // LORA RESET
int PIN_LORA_NSS = 42;   // LORA SPI CS
int PIN_LORA_SCLK = 43;  // LORA SPI CLK
int PIN_LORA_MISO = 45;  // LORA SPI MISO
int PIN_LORA_DIO_1 = 47; // LORA DIO_1
int PIN_LORA_BUSY = 46;  // LORA SPI BUSY
int PIN_LORA_MOSI = 44;  // LORA SPI MOSI
int RADIO_TXEN = 37;   // LORA ANTENNA TX ENABLE
int RADIO_RXEN = 39;   // LORA ANTENNA RX ENABLE
// Replace PIN_SPI_MISO, PIN_SPI_SCK, PIN_SPI_MOSI with your
SPIClass SPI_LORA(NRF_SPIM2, PIN_LORA_MISO, PIN_LORA_SCLK, PIN_LORA_MOSI);

// Function declarations
void OnTxDone(void);
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);
void OnTxTimeout(void);
void OnRxTimeout(void);
void OnRxError(void);
void OnCadDone(bool cadResult);


#ifdef NRF52_SERIES
#define LED_BUILTIN 35
#endif

// Define LoRa parameters
#define RF_FREQUENCY 868300000  // Hz
#define TX_OUTPUT_POWER 22		// dBm
#define LORA_BANDWIDTH 0		// [0: 125 kHz, 1: 250 kHz, 2: 500 kHz, 3: Reserved]
#define LORA_SPREADING_FACTOR 7 // [SF7..SF12]
#define LORA_CODINGRATE 1		// [1: 4/5, 2: 4/6,  3: 4/7,  4: 4/8]
#define LORA_PREAMBLE_LENGTH 8  // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT 0   // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false
#define RX_TIMEOUT_VALUE 3000
#define TX_TIMEOUT_VALUE 3000

#define BUFFER_SIZE 64 // Define the payload size here

static RadioEvents_t RadioEvents;
static uint16_t BufferSize = BUFFER_SIZE;
static uint8_t RcvBuffer[BUFFER_SIZE];
static uint8_t TxdBuffer[BUFFER_SIZE];
static bool isMaster = true;
const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";

time_t timeToSend;

time_t cadTime;

uint8_t pingCnt = 0;
uint8_t pongCnt = 0;
uint8_t packet[] = {'1','2','3','4','5'};
void setup()
{
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
  while ( !Serial ) delay(10);   // for nrf52840 with native usb
	//Serial.println("=====================================");
	//Serial.println("SX126x PingPong test");
	//Serial.println("=====================================");

	// Initialize the LoRa chip
	//Serial.println("Starting lora_hardware_init");
	lora_hardware_init(hwConfig);

	// Initialize the Radio callbacks
	RadioEvents.TxDone = OnTxDone;
	RadioEvents.RxDone = OnRxDone;
	RadioEvents.TxTimeout = OnTxTimeout;
	RadioEvents.RxTimeout = OnRxTimeout;
	RadioEvents.RxError = OnRxError;
	RadioEvents.CadDone = OnCadDone;

	// Initialize the Radio
	Radio.Init(&RadioEvents);

	// Set Radio channel
	Radio.SetChannel(RF_FREQUENCY);
  Serial.println("TX mode");
	// Set Radio TX configuration
	Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
					  LORA_SPREADING_FACTOR, LORA_CODINGRATE,
					  LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
					  true, 0, 0, LORA_IQ_INVERSION_ON, TX_TIMEOUT_VALUE);
  int i = 0;                               
  for(i;i<10;i++)
  {
      Radio.Send(packet,5);
      delay(2000);
  }
  Serial.println("TX Done");
  delay(2000);
  Serial.println("RX mode");
	// Set Radio RX configuration
	Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
					  LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
					  LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
					  0, true, 0, 0, LORA_IQ_INVERSION_ON, true);

	// Start LoRa
	Serial.println("Starting Radio.Rx");
	Radio.Rx(0);

	timeToSend = millis();
  delay(15000);
  Serial.println("Ble Example");
  Serial.println("----------------------------------------\n");
  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  Bluefruit.setName("RAK4630");

  // Set up and start advertising
  startAdv();

  Serial.println("Advertising is started"); 
}

void loop()
{
	// Handle Radio events
	Radio.IrqProcess();

	// We are on FreeRTOS, give other tasks a chance to run
	//delay(100);
	//yield();
}
void startAdv(void)
{   
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addName();

  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html
   */
  Bluefruit.Advertising.setStopCallback(adv_stop_callback);
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in units of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(ADV_TIMEOUT);      // Stop advertising entirely after ADV_TIMEOUT seconds 
}
void adv_stop_callback(void)
{
  Serial.println("Advertising time passed, advertising will now stop.");
}
/**@brief Function to be executed on Radio Tx Done event
 */
void OnTxDone(void)
{
	//Serial.println("OnTxDone");
	Radio.Rx(RX_TIMEOUT_VALUE);
}

/**@brief Function to be executed on Radio Rx Done event
 */
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
	//Serial.println("OnRxDone");
	delay(10);
	BufferSize = size;
	memcpy(RcvBuffer, payload, BufferSize);

	Serial.printf("RssiValue=%d dBm, SnrValue=%d\n", rssi, snr);

	for (int idx = 0; idx < size; idx++)
	{
		Serial.printf("%02X ", RcvBuffer[idx]);
	}
	Serial.println("");


//	if (isMaster == true)
//	{
//		if (BufferSize > 0)
//		{
//			if (strncmp((const char *)RcvBuffer, (const char *)PongMsg, 4) == 0)
//			{
//				Serial.println("Received a PONG in OnRxDone as Master");
//
//				// Wait 500ms before sending the next package
//				delay(500);
//
//				// Check if our channel is available for sending
//				Radio.Standby();
//				Radio.SetCadParams(LORA_CAD_08_SYMBOL, LORA_SPREADING_FACTOR + 13, 10, LORA_CAD_ONLY, 0);
//				cadTime = millis();
//				Radio.StartCad();
//				// Sending next Ping will be started when the channel is free
//			}
//			else if (strncmp((const char *)RcvBuffer, (const char *)PingMsg, 4) == 0)
//			{ // A master already exists then become a slave
//				Serial.println("Received a PING in OnRxDone as Master");
//
//				isMaster = false;
//				Radio.Rx(RX_TIMEOUT_VALUE);
//			}
//			else // valid reception but neither a PING or a PONG message
//			{	// Set device as master and start again
//				isMaster = true;
//				Radio.Rx(RX_TIMEOUT_VALUE);
//			}
//		}
//	}
//	else
//	{
//		if (BufferSize > 0)
//		{
//			if (strncmp((const char *)RcvBuffer, (const char *)PingMsg, 4) == 0)
//			{
//				Serial.println("Received a PING in OnRxDone as Slave");
//
//
//				// Check if our channel is available for sending
//				Radio.Standby();
//				Radio.SetCadParams(LORA_CAD_08_SYMBOL, LORA_SPREADING_FACTOR + 13, 10, LORA_CAD_ONLY, 0);
//				cadTime = millis();
//				Radio.StartCad();
//				// Sending Pong will be started when the channel is free
//			}
//			else // valid reception but not a PING as expected
//			{	// Set device as master and start again
//				Serial.println("Received something in OnRxDone as Slave");
//
//				isMaster = true;
//				Radio.Rx(RX_TIMEOUT_VALUE);
//			}
//		}
//	}
}

/**@brief Function to be executed on Radio Tx Timeout event
 */
void OnTxTimeout(void)
{
//	// Radio.Sleep();
//	Serial.println("OnTxTimeout");
//
//	digitalWrite(LED_BUILTIN, LOW);
//
//	Radio.Rx(RX_TIMEOUT_VALUE);
}

/**@brief Function to be executed on Radio Rx Timeout event
 */
void OnRxTimeout(void)
{
//	Serial.println("OnRxTimeout");
//
//
//	digitalWrite(LED_BUILTIN, LOW);
//
//	if (isMaster == true)
//	{
//		// Wait 500ms before sending the next package
//		delay(500);
//
//		// Check if our channel is available for sending
//		Radio.Standby();
//		Radio.SetCadParams(LORA_CAD_08_SYMBOL, LORA_SPREADING_FACTOR + 13, 10, LORA_CAD_ONLY, 0);
//		cadTime = millis();
//		Radio.StartCad();
//		// Sending the ping will be started when the channel is free
//	}
//	else
//	{
//		// No Ping received within timeout, switch to Master
//		isMaster = true;
//		// Check if our channel is available for sending
//		Radio.Standby();
//		Radio.SetCadParams(LORA_CAD_08_SYMBOL, LORA_SPREADING_FACTOR + 13, 10, LORA_CAD_ONLY, 0);
//		cadTime = millis();
//		Radio.StartCad();
//		// Sending the ping will be started when the channel is free
//	}
}

/**@brief Function to be executed on Radio Rx Error event
 */
void OnRxError(void)
{
//	Serial.println("OnRxError");
//
//	digitalWrite(LED_BUILTIN, LOW);
//
//	if (isMaster == true)
//	{
//		// Wait 500ms before sending the next package
//		delay(500);
//
//		// Check if our channel is available for sending
//		Radio.Standby();
//		Radio.SetCadParams(LORA_CAD_08_SYMBOL, LORA_SPREADING_FACTOR + 13, 10, LORA_CAD_ONLY, 0);
//		cadTime = millis();
//		Radio.StartCad();
//		// Sending the ping will be started when the channel is free
//	}
//	else
//	{
//		Radio.Rx(RX_TIMEOUT_VALUE);
//	}
}

/**@brief Function to be executed on Radio Rx Error event
 */
void OnCadDone(bool cadResult)
{
//	time_t duration = millis() - cadTime;
//	if (cadResult)
//	{
//		Serial.printf("CAD returned channel busy after %ldms\n", duration);
//
//		Radio.Rx(RX_TIMEOUT_VALUE);
//	}
//	else
//	{
//		Serial.printf("CAD returned channel free after %ldms\n", duration);
//
//		if (isMaster)
//		{
//			Serial.println("Sending a PING in OnCadDone as Master");
//
//			// Send the next PING frame
//			TxdBuffer[0] = 'P';
//			TxdBuffer[1] = 'I';
//			TxdBuffer[2] = 'N';
//			TxdBuffer[3] = 'G';
//		}
//		else
//		{
//			Serial.println("Sending a PONG in OnCadDone as Slave");
//
//			// Send the reply to the PONG string
//			TxdBuffer[0] = 'P';
//			TxdBuffer[1] = 'O';
//			TxdBuffer[2] = 'N';
//			TxdBuffer[3] = 'G';
//		}
//		// We fill the buffer with numbers for the payload
//		for (int i = 4; i < BufferSize; i++)
//		{
//			TxdBuffer[i] = i - 4;
//		}
//
//		Radio.Send(TxdBuffer, BufferSize);
//	}
}

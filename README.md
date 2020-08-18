# LPWAN Evaluation Boards

### 1 Introduction
These evaluation boards are designed to give you a quick start to explore the LoRa/LPWAN world.
Based on our WisBlock product line, they are 30 x 60 mm small and support USB power supply, 3.7V LiPo battery and 5V solar charging.    

These modules can be extended with most of our WisBlock Sensor and WisBlock IO modules. Check out the available WisBlock IO and WisBlock Sensor modules in the [RAKwireless Store](https://store.rakwireless.com/)

To make it easy for you to start, these modules are pre-flashed with a basic firmware that offers an AT command interface over the USB connector to setup and connect them to a LPWAN gateway and server.

Please check out our [documentation](https://doc.rakwireless.com) pages for more details.

There are three different LPWAN Evalution Boards available:   

----
## RAK4260 LPWAN Evaluation Board
The RAK4260 is based on the Microchip ATSAMR34J18 MCU with an integrated LoRa® transceiver, that complies with the LoRaWan® 1.0.2 protocols.    
![image](/res/RAK4260-1.png)    
### Key Features
- ATSAMR34J18B MCU
  - 32-bit Arm® Cortex M0+ MCU
  - UART/I2C/GPIO
  - 256KB Flash and 40 KB RAM
- Integrated LoRa® transceiver
  - Frequency range: 862 to 1020 MHz
  - Receive Sensitivity down to -148 dBm
  - Maximum Transmit Power up to 20 dBm

The RAK4260 is available for the following LPWAN regions:
| | |
|-|-|
|EU868|IN865|
|US915|AU915|
|RU864|AS923|
|KR920||
### RAK4260 Setup and Software sample
Learn in [RAK4260](/RAK4260) how to setup the ArduinoIDE and check out our sample application.     
More details can be found in our [Web Documentation](https://doc.rakwireless.com/rak4260-lora-evaluation-board/overview)    

----
## RAK4600 LPWAN Evaluation Board
The 4600 is based on the nRF52832  MCU and a SX1276 LoRa® transceiver, that complies with the LoRaWan® 1.0.2 protocols.    
![image](/res/RAK4600-s.jpg)    
### Key Features
- nRF52832 MCU
  - 64MHz Cortex-M4 with FPU
  - UART/I2C/GPIO
  - 512 kB flash
  - 64kB RAM
- SX1276 LoRa® transceiver
  - Frequency range: 863–870MHz (EU) / 902–928MHz (US), ISM and SRD systems
  - Ultra-Low Power Consumption 1.5uA in sleep mode
- 8 mBit NOR flash

The RAK4200 is available for the following LPWAN regions:
| | |
|-|-|
|EU433|IN865|
|EU868|AU915|
|US915|AS923|
|CN470|KR920|
|RU864||
### RAK4600 Setup and Software sample
Learn in [RAK4600](/RAK4600/arduino) how to setup the ArduinoIDE and check out our sample application.     
Learn in [RAK4600](/RAK4600/micropython) how to program the RAK4600 in microPython.     
More details can be found in our [Web Documentation](https://doc.rakwireless.com/rak4600-lora-evaluation-board/overview)    

----
## RAK811 LPWAN Evaluation Board
The RAK811 is based on the STM32L151 MCU and a SX1276 LoRa® transceiver, that complies with the LoRaWan® 1.0.2 protocols.    
![image](/res/RAK4200-1.png)    
### Key Features
- STM32L151 MCU
  - ARM 32-bit Cortex – M3
  - UART/I2C/GPIO
  - 128KB flash memory with ECC
  - 16KB RAM
- SX1276 LoRa® transceiver
  - Frequency range: 863–870MHz (EU) / 902–928MHz (US), ISM and SRD systems
  - Ultra-Low Power Consumption 1.5uA in sleep mode

The RAK811 is available for the following LPWAN regions:
| | |
|-|-|
|EU433|IN865|
|EU868|AU915|
|US915|AS923|
|CN470|KR920|
|RU864||
### RAK811 Setup and Software sample
Learn in [RAK811](/RAK811) how to setup the ArduinoIDE and check out our sample application.     
More details can be found in our [Web Documentation](https://doc.rakwireless.com/rak811-lora-evaluation-board)    



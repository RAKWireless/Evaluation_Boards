# Change log
2020.4.10

1. Support shtc3,lps22hb,bme689
2. Support GPIO

# RAK4600 Arduino
2020.1.4

## Introduction

RAK4600 LoRa Module includes an nRF52832 MCU and an SX1276 LoRa chip. Its RF communication capabilities (LoRa+BLE) make it suitable for a variety of applications in the IoT field.More details can be found in this：

[RAK4600 LoRa Module - RAKwireless Knowledge Hub](https://doc.rakwireless.com/datasheet/rakproducts/rak4600-lora-module-datasheet).

RAK4600 can be ordered in RAK store:

https://store.rakwireless.com/products/rak4600-lora-module?variant=31767268851757


## BSP Installation

The following describes how the module builds the development environment in Arduino IDE and runs the demo project.


1. What is Arduino?
     If you know little about Arduino, please have a look below: 
     [https://www.arduino.cc/](https://www.arduino.cc/) 

2. You have known Arduino.  Install the IDE first: 
     [https://www.arduino.cc/en/Main/Software](https://www.arduino.cc/en/Main/Software) 

3. Which BSP is used? 
     RAK4600 is based on nrf52832, therefore Arduino Core for Adafruit Bluefruit nRF52 Boards is suitable for RAK4600. 

     Refer to the following link to install Adafruit Bluefruit nRF52 in Arduino.
     [https://github.com/adafruit/Adafruit_nRF52_Arduino](https://github.com/adafruit/Adafruit_nRF52_Arduino) 

     After installation, select the board according to the figure below.

     ![nRF52](/RAK4600/image/Select%20development%20board.png) 

4. Download the gpio map of RAK4600 from:
   [https://github.com/RAKWireless/RAK4600_Arduino](https://github.com/RAKWireless/RAK4600_Arduino) 
   It contains gpio maping, serial tool, demo project and Softdevice hex.

   - feather_nrf52832: GPIO maping of RAK4600.
   - example: lorawan and ble demo project.
   - serial tool: serial tool on PC.
   - feather_nrf52832_bootloader.hex: Includes sotfdevice and bootloader.

5. Replace your folder with our folder `feather_nrf52832`, your file path maybe 

   > %APPDATA%\Local\Arduino15\packages\adafruit\hardware\nrf52\0.14.6\variants

6. The last step is program the bootloader hex file `feather_nrf52832_bootloader.hex` to RAK4600 with Jlink or DAP, etc.

At this point, the development environment is ready.



## BootLoader

The bootloader of RAK4600 supports the serial port upgrade program. Use the Arduino IDE to upgrade as follows.

1. Connect the pin 18 of MCU to the GND.
2. After restarting the MCU, disconnect pin 18 from GND. The current state is bootloader mode.
3. Click the `Upload` button to update the firmware to RAK4600.

**Note**：

​    1. If you use a WisBLOCK board (RAK5005), the silk screen of pin 18 on the back of the board is "**RX1**".

![DFU](/res/4600.jpg)

​    2. You can also refer to the following link to customize your own bootloader.[Adafruit_nRF52_Bootloader](https://github.com/adafruit/Adafruit_nRF52_Bootloader) 



![DFU](/RAK4600/image/DFU%20Log.png)



## BLE example

Adafruit provides many BLE demos, we open them from File->Examples.

1. Open one of the demos, File->Examples->Adafruit Bluefruit nRF52 Libraries->peripheral->bleuart.

![Project](/RAK4600/image/ble_examples.png)

2. Update firmware with bootloader, View the log through the serial port on the PC.

3. We need to install an app in the mobile phone to connect with the module.

   Refer to the following link for installation. [nRF Connect for Mobile - nordicsemi.com](https://www.nordicsemi.com/Software-and-tools/Development-Tools/nRF-Connect-for-mobile)  

4. After the installation is completed, open Bluetooth and app. Scan and connect a device named "bluefruit 52". You can communicate with your device via Bluetooth.

![ble_scan](/RAK4600/image/ble_scan.png) 

6. Here are the logs of mobile phone and PC。

![ble_mobile_app](/RAK4600/image/ble_mobile_app.jpg) 

![ble_pc_tool](/RAK4600/image/ble_pc_tool.png) 



## LoRa example

RAK4600 uses open source protocol stack to realize lorawan communication.

1. What lorawan lib is used?

     LMIC is a LoraWAN-MAC-in-C library, adapted to run under the Arduino environment.

     Refer to the following link to install it.

     [https://github.com/mcci-catena/arduino-lmic](https://github.com/mcci-catena/arduino-lmic) 

![lmic](https://github.com/RAKWireless/RAK4600_Arduino/raw/master/image/LMIC.png) 

2. Open the demo project `RAK4600_LoRaWAN_Demo.ino` in Arduino IDE. Modify the device parameters to your own.

     Notice:The APPEUI and DEVEUI must be in **little-endian format**.

![Device Parameter](/RAK4600/image/LoRa_OTAA_Parameters.png) 

3. Update firmware with bootloader. When download ok, the RAK4600 will be auto join LoRaWAN and send data.

![Run information](/RAK4600/image/running%20information.png) 


Please note that the default region is US915, if you want to use another region like EU868, you should open the file "lmic_project_config.h" in the folder <...\Documents\Arduino\libraries\arduino-lmic-master\project_config>, and modify the region to the one you want to use.

![Region](/RAK4600/image/Arduino-LoRa%20region%20configuration.png)


## BLE + LoRa example

BLE and LoRa demo is `RAK4600_ble_scan_lora.ino`, Modify the lora parameters to your own, compile and upload it as before.

We can see the following logs.

![Run information](/RAK4600/image/ble%2Blora.png)

## Support peripheral

LoraWAN, BLE, lis3dh, opt3001

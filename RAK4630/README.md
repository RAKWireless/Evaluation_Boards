How to Use RAK4630 with Arduino IDE
== 
2020.6.11

RAK4639 is based on nRF52840, lora+ble module of wisblock. Here I will introduce you how to use it with wisblock. The base board now support usb for download in Arduino IDE. Of course battery is needed.


# Burn bootloader

Burn bootloader/boot.hex with Jlink like below:
![](https://github.com/RAKWireless/Wisblock/blob/master/res/4630_download.png)


# Install BSP

After download bootloader, Install the BSP library. We use Adafruit nRF52 by Adaruit in Arduino now.

![](https://github.com/RAKWireless/Wisblock/blob/master/res/4630%20lib.png)

After install, user should replace our pin map header /bsp/variant.h to the one in Adafruit nRF52. It maybe like below:

C:\Program Files (x86)\Arduino\hardware\Adafruit\Adafruit_nRF52_Arduino\variants\pca10056

And you should find yours path and replace.

# Install lora lib

We supply a lorawan lib (lorawan/SX126x_Arduino_base) which support ClassA and Class C, region incluing EU868,US915,CN470,AS923,KR920,IN865. Use should add it in Arduino like below:

![](https://github.com/RAKWireless/Wisblock/blob/master/res/4630_install%20lib.png)

# Application

We supply many application incluing lora, ble, sensors, IO like RS485, 0-5V,4-20mA e.g

# Support 

LoraWAN BLE Cellular Wifi Sensor IO

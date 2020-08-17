

# Change log 
2020.08.17 
Reuse SW example and tutorials for RAK4200 (uses same MCU and LoRa Transceiver as RAK811) bernd.giesecke@rakwireless.com

2020.4.20
1. Support sensor,like bme680, lis3dh, lps22hb, max_7q, opt3001, shtc3
2. Support IO, like RS485, GPIO

# RAK4200_LoRaWAN_Arduino 
2020.08.17 
Rename for RAK4200 (uses same MCU and LoRa Transceiver as RAK811) bernd.giesecke@rakwireless.com

2019.12.30


Firstly, thank you very much to .@sabas1080 for his contribution on RAK4200/RAK811 LoRa Tracker board + Arduino.

1. What is Arduino?
If you know little about Arduino, please have a look below:  
[https://www.arduino.cc/](https://www.arduino.cc/)

2. You have known Arduino.  Install the IDE first:  
[https://www.arduino.cc/en/Main/Software](https://www.arduino.cc/en/Main/Software)

3. What lib is used?  
RAK4200/RAK811 is based on STM32L151.Therefore Arduino Core for Arduino_Core_STM32 is suitable for RAK4200/RAK811.

4. How to install Arduino_Core_STM32 in Arduino?  
[https://github.com/stm32duino/Arduino_Core_STM32](https://github.com/stm32duino/Arduino_Core_STM32)  
add "https://github.com/stm32duino/BoardManagerFiles/raw/master/STM32/package_stm_index.json" to **Additional Boards Manager URLs:** option  
![install Arduino_Core_STM32](/res/image/install%20Arduino_Core_STM32.png)  
Check the other lib below install or not?
![Check Cores](/res/image/Check%20Cores.png)
Then install the BSP for STM32.
If install completed as following:
![install board](/res/image/install%20Board.png)
5. Download the LoRaWAN library from below. It is a very tiny lorawan protocol only support ABP mode for now, if you want to use OTAA, you can develop it yourself in the lib:  
    https://github.com/adafruit/TinyLoRa 

    ![LoRaWAN library](/res/lib.png)


6. OK, development environment is finished. Open the Arduino_RAK4200_LoRaWAN.ino，config 3 parameters for LoRaWAN_ABP. The demo is test in US915. If you want to define other region, you should change in the lib, the path in your pc maybe C:\Users\..\Documents\Arduino\libraries\TinyLoRa\TinyLoRa.h.  

![config 3 parameters](/res/config.png)  

Or you can add region define like below:

![config 3 parameters](/res/define.png)


7. Compile and download via Arduino IDE.You can download firware to board via Arduino, What need?

    a. Install STM32CubeProgrammer from https://www.st.com/en/development-tools/stm32cubeprog.html

    b. Choose in Arduino: Tools->Upload method:"STM32CubeProgrammer(Serial)"

    c. Connect Boot0 to VDD, you can find it on back of wisblock, then reset
    
    ![config 3 parameters](/res/811.jpg)

    d. Click Download in Arduino, it will automatically download. 


8. When download ok, the Gateway and server are working properly.  
The RAK4200/RAK811 will be auto send data to server:
![Joinning LoRaWAN](/res/log.png)  
9. We test it in Lora Server, receive data per 10s:  
![TTN data](/res/send%20data.png)



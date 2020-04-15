

# Change log 
2020.4.20

1. Support sensor,like bme680, lis3dh, lps22hb, max_7q, opt3001, shtc3
2. Support IO, like RS485, GPIO

# RAK811_LoRaWAN_Arduino 
2019.12.30


Firstly, thank you very much to .@sabas1080 for his contribution on RAK811 LoRa Tracker board + Arduino.

1. What is Arduino?
If you know little about Arduino, please have a look below:  
[https://www.arduino.cc/](https://www.arduino.cc/)

2. You have known Arduino.  Install the IDE first:  
[https://www.arduino.cc/en/Main/Software](https://www.arduino.cc/en/Main/Software)

3. What lib is used?  
RAK811 is based on STM32L151.Therefore Arduino Core for Arduino_Core_STM32 is suitable for RAK811.

4. How to install Arduino_Core_STM32 in Arduino?  
[https://github.com/stm32duino/Arduino_Core_STM32](https://github.com/stm32duino/Arduino_Core_STM32)  
add "https://github.com/stm32duino/BoardManagerFiles/raw/master/STM32/package_stm_index.json" to **Additional Boards Manager URLs:** option  
![install Arduino_Core_STM32](https://github.com/RAKWireless/Wisblock/blob/master/res/image/install%20Arduino_Core_STM32.png)  
Check the other lib below install or not?
![Check Cores](https://github.com/RAKWireless/Wisblock/blob/master/res/image/Check%20Cores.png)
Then install the BSP for STM32.
If install completed as following:
![install board](https://github.com/RAKWireless/Wisblock/blob/master/res/image/install%20Board.png)
5. Download the LoRaWAN library from:  
[https://github.com/mcci-catena/arduino-lmic#selecting-the-lorawan-region-configuration](https://github.com/mcci-catena/arduino-lmic#selecting-the-lorawan-region-configuration)  
Add it to Arduino IDE:  
![LoRaWAN library](https://github.com/RAKWireless/Wisblock/blob/master/res/image/Install%20LoRaWAN%20library.png)
6. OK, development environment is finished. Open the Arduino_RAK811_LoRaWAN.ino，config 3 parameters for LoRaWAN_OTAA:  
![config 3 parameters](https://github.com/RAKWireless/Wisblock/blob/master/res/image/config%203%20parameters%20for%20LoRaWAN_OTAA.png)  
And set Pins mapping:  
![Pin mapping](https://github.com/RAKWireless/Wisblock/blob/master/res/image/Pin%20mapping.png)

7. Compile it,ingnore the Waring:  
![compile success](https://github.com/RAKWireless/Wisblock/blob/master/res/image/Compile%20success.png)  
You can download firware to board via Arduino, What need?
1. Install STM32CubeProgrammer from https://www.st.com/en/development-tools/stm32cubeprog.html
2. Choose in Arduino: Tools->Upload method:"STM32CubeProgrammer(Serial)"
3. Connect Boot0 to VDD, you can find it on back of wisblock, then reset
4. Click Download in Arduino, it will automatically download. 

8. When download ok, the Gateway and server are working properly.  
The RAK811 will be auto join LoRaWAN:
![Joinning LoRaWAN](https://github.com/RAKWireless/Wisblock/blob/master/res/image/Joined.png)  
TTN receive data:  
![TTN data](https://github.com/RAKWireless/Wisblock/blob/master/res/image/TTN%20receive%20data.png)

Important Note:


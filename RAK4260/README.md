How to Use RAK4260 with Arduino IDE
==

RAK4260 is based on ATSAML21J18B from microchip, one lora module of wisblock. Before use it, User should prepare something. Our friend shows how to use it in their base board, more details is at :

https://www.hackster.io/electronic-cats/how-to-use-rak4260-with-arduino-ide-4bcff2#comments

Here I will introduce you how to use it with wisblock. Our board is like below. The base board now include uart and usb com. The usb com is one component of wisblock like sensors. In Arduino, we use usb for test, battery is needed.

![](https://github.com/RAKWireless/Wisblock/blob/master/RAK4260/resource/board.jpg) 

# Burn bootloader

Burn bootloader-bast-wan-v3.4.0.bin with Jlink like below:
![](https://github.com/RAKWireless/Wisblock/blob/master/RAK4260/resource/download.png)


# Install BSP

After download bootloader, Install the BSP library.To add board support for our products, start Arduino and open the Preferences window (File > Preferences). Now copy and paste the following URL into the 'Additional Boards Manager URLs' input field:

https://electroniccats.github.io/Arduino_Boards_Index/package_electroniccats_index.json
If there is already an URL from another manufacturer in that field, click the button at the right end of the field. This will open an editing window allowing you to paste the above URL onto a new line.
Press the "OK" button.

Press the "OK" button.
Open the "boards manager" that is in tools --> Board --> board manager.

Open the "boards manager" that is in tools --> Board --> board manager.
Now write "Electronic Cats" (without quotes) in the search bar.

Now write "Electronic Cats" (without quotes) in the search bar.
Click in install for "ElectronicCatsSAM DLC", jus wait to finish to install and only close the window.

![](https://github.com/RAKWireless/Wisblock/blob/master/RAK4260/resource/lib.png)

# Install lora lib

https://github.com/BeelanMX/Beelan-LoRaWAN

# Compile as below

![](https://github.com/RAKWireless/Wisblock/blob/master/RAK4260/resource/bastwan.png)

You will see the information via usb in serial tool.



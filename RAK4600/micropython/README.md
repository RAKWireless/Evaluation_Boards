# Change log
2020.4.10

1. Support sensor, lis3dh, opt3001,bme680
2. Support GPIO

# Micropython & LoRaWAN on RAK4600 
2020.2.4

Glad to announce we have run micropython on RAK4600, which is a new try to combine micropython and LoRaWAN!

If you know little micropython, maybe you can see it first:
http://docs.micropython.org/en/latest/

Here you get the point of micropython, let’s begin. We provide the LoRaWAN on python version, firmware, app python script and serial tool.

LoRaWAN is a tiny loRaWAN realized by python from below and I have modified bsp part in order to adapt to RAK4600. It just supports ABP with TTN now. (https://github.com/adafruit/Adafruit_CircuitPython_TinyLoRa)

## Burn firmware

First burn the micropython_rak4600.hex with jlink like below:
![](/RAK4600/image/burn.png)

## Connect to serial

Then connect board(do not forget antenna) to your PC with USB, open the serial tool, press reset button, it will show:
![](/RAK4600/image/serial.png)

Congradulations! It has run normally! Because our hex includes the LoRaWAN. What you need is just push the lora.py to board file system with ampy. How to install ampy? You can refer to this:

https://forum.rakwireless.com/t/micropython-on-rak5010/1148

OK, before push, you should change some parameters with your application of TTN like below:

DEVADDR = bytearray([0x26, 0x01, 0x1F, 0xCF])

NWKEY = bytearray([0xC0, 0x24, 0x7D, 0x95, 0x3A, 0x68, 0x30, 0x8B, 0xA2, 0x80, 0xC3, 0x00, 0x16, 0x69, 0x82, 0x5C])

APP = bytearray([0x71, 0xB3, 0x42, 0x2B, 0x67, 0xA7, 0x55, 0x10,0x4E, 0x6F, 0xAF, 0x1A, 0xF9, 0xA9, 0x90, 0x51])

REGION = “EU”

Notes: “EU” for eu868, “AU” for au915, “US” for us915, “AS” for as920

![](/RAK4600/image/repl.png)

Everything is OK and ready to send your data to TTN! In serial tool, input below:
import lora

lora.send(“123”)

![](/RAK4600/image/test.png)

After Tx Finish, you will see it in TTN like below:

![](/RAK4600/image/ttn.png)

Done! Very easy.

For developers who want to do some personalized functions like OTAA, I advised you compile your own hex. You can get the code from below. Change the Uart TX and RX to suit for RAK4600 and put the LoRaWAN to \ports\nrf\freeze. Compile with make BOARD=pca10040 SD=s132 FROZEN_MPY_DIR=freeze. The Hex just includes application, without softdevice:


Notes:

Micropython original thing is still work, like CTRL+D for restart
It runs the ble_uart_nus demo as periphral
User can do any change they want.
We are keeping study how to run script automatically, other than just in REPL mode. If you make it true, it is nice to share with us and other developers

Your TTN config should like this. Remember disable the Frame counter checks, because our demo disable it in lora.py:
![](/RAK4600/image/abp.png)

Here also shows how to use with WisBlock sensor

1. Because of bme680 is large to REPL, it is in the firmware. Just use environment_test.py to test.
2. All sensor script is independent. Use REPL to push what you need.
3. lora_light.py shows an example how to send light data to TTN.

## Support peripheral

LoraWAN, BLE, shtc3, lps22hb

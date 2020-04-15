

import time
import math
from machine import Pin
from machine import UART
from machine import I2C


#i2c obj
i2c=I2C(1,12,13)



#lps22hb init addr-0x5c
def init():
	buf=bytearray([0x50])
	i2c.writeto_mem(0x5c,0x10,buf)
	lps22hb_id=i2c.readfrom_mem(0x5c,0x0F,1)
	lps22hb_id=''.join(['%02X' %x  for x in lps22hb_id])
	print('I2C device lps22hb init and id is 0x',lps22hb_id)

def get_data():
	pre_xl=i2c.readfrom_mem(0x5c,0x28,1)
	pre_xl=int.from_bytes(pre_xl,'big')
	pre_l=i2c.readfrom_mem(0x5c,0x29,1)
	pre_l=int.from_bytes(pre_l,'big')
	pre_h=i2c.readfrom_mem(0x5c,0x2a,1)
	pre_h=int.from_bytes(pre_h,'big')
	pre = (pre_h<<16)|(pre_l<<8)|pre_xl
	if pre & 0x00800000:
		pre |= 0xFF000000

	pre = pre/4096.0
	print('lps22hb:pressure=%f Pa'%pre)











import time
import math
from machine import Pin
from machine import UART
from machine import I2C


#i2c obj
i2c=I2C(1,12,13)



#opt3001 init addr-0x44
def init():
	buf=bytearray([0xCC,0x10])
	i2c.writeto_mem(0x44,0x01,buf)
	manufacture_id=i2c.readfrom_mem(0x44,0x7E,2)
	num=int.from_bytes(manufacture_id,'big')
	print('I2C device opt3001 init and manufacture id is %d'%num)
	device_id=i2c.readfrom_mem(0x44,0x7F,2)
	num=int.from_bytes(device_id,'big')
	print('I2C device opt3001 init and device id is %d'%num)

def get_data():
	buf=i2c.readfrom_mem(0x44,0x00,2)
	light=int.from_bytes(buf,'big')
	m=light & 0x0FFF
	e=(light & 0xF000) >> 12
	h= math.pow(2,e)
	light = m*(0.01 * h)
	print('opt3001:light strength = %f'%light)









import utime
from ulora import TTN, uLoRa
# Refer to device pinout / schematics diagrams for pin details
LORA_CS = const(4)
LORA_SCK = const(7)
LORA_MOSI = const(5)
LORA_MISO = const(6)
LORA_IRQ = const(27)
LORA_RST = const(3)
LORA_DATARATE = "SF7BW125"  # Choose from several available
# From TTN console for device
DEVADDR = bytearray([0x26, 0x01, 0x1F, 0xCF])
NWKEY = bytearray([0xC0, 0x24, 0x7D, 0x95, 0x3A, 0x68, 0x30, 0x8B,
                   0xA2, 0x80, 0xC3, 0x00, 0x16, 0x69, 0x82, 0x5C])
APP = bytearray([0x71, 0xB3, 0x42, 0x2B, 0x67, 0xA7, 0x55, 0x10,
                 0x4E, 0x6F, 0xAF, 0x1A, 0xF9, 0xA9, 0x90, 0x51])

REGION = "EU"

TTN_CONFIG = TTN(DEVADDR, NWKEY, APP, country="EU")
FPORT = 1

lora = uLoRa(
    cs=LORA_CS,
    sck=LORA_SCK,
    mosi=LORA_MOSI,
    miso=LORA_MISO,
    irq=LORA_IRQ,
    rst=LORA_RST,
    ttn_config=TTN_CONFIG,
    datarate=LORA_DATARATE,
    fport=FPORT
)

# ...Then send data as bytearray

def send(data):
	b = bytearray(data)
	print("send data:", b)
	lora.send_data(b, len(b), lora.frame_counter)

def config(devaddr,nwskey,appkey,region):
	DEVADDR = devaddr
	NWKEY = nwskey
	APP = appkey
	REGION = region
	print("DEVADDR:", DEVADDR)
	print("NWKEY:", NWKEY)
	print("APP:", appkey)
	print("REGION:", REGION)	

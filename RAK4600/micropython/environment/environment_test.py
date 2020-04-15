import bme680
import time

sensor = bme680.BME680()
# These oversampling settings can be tweaked to
# change the balance between accuracy and noise in
# the data.
sensor.set_humidity_oversample(bme680.OS_2X)
sensor.set_pressure_oversample(bme680.OS_4X)
sensor.set_temperature_oversample(bme680.OS_8X)
sensor.set_filter(bme680.FILTER_SIZE_3)
sensor.get_sensor_data()
output = "{} C, {} hPa, {} RH, {} RES,".format(sensor.data.temperature,sensor.data.pressure,sensor.data.humidity,sensor.data.gas_resistance)
print(output)
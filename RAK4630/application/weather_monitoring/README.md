# This demo is a weather LoraWAN example

 Type: OTAA
 
 Region: EU868
 
 Function: send environment data to server period
 
 Notes: The data format will be like 1+temperature(2 bytes)+ humidity(2 bytes)+
 pressure(4 bytes)+light(2 bytes) on server. If use US915, do not set to 
 DR0, the payload will be short for information.


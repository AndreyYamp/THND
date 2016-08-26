# THND
Temperature And Humidity Notification Device

This project aimed to gather experience in Arduino programming and make cheap and useful device to monitor temperature and humidity in a room. In future we plan to add notification function in case one of the parameters goes out of defined boundaries.
This device has a web-server to monitor values.
The web-server works only in local network at the moment with IP: 192.168.0.115.
Device includes only digital temperature sensor DS18B20 therefore web-server can monitor the temperature only.

To use arduino_uip-master library it should be moved to .zip archive before and then added to Arduino IDE.
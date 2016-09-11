# THND
Temperature And Humidity Notification Device

This project aimed to gather experience in Arduino programming and make cheap and useful device to monitor temperature and humidity in a room. In future we plan to add notification function in case one of the parameters goes out of defined boundaries.
This device has a web-server to monitor values.
The web-server works only in local network at the moment with IP: 192.168.0.115.
Device includes only analog temperature sensor therefore web-server can monitor the temperature only.
Exceeding temperature limits, THND sends e-mail via Gmail.
Signal from thermistor converted with Steinhart–Hart equation.
The application sports anti-spam logic that prevents notification spam in case of longer sensor triggering.

Hardware:
Arduino Mega 2560

Ethernet shield based on ENC28J60

Analog temperature sensor MF52E B 3950K with 10Kohm resistor

To use arduino_uip-master and Temboo libraries they should be moved to .zip archive before and then added to Arduino IDE separately.

// Include libraries for ethernet shield
#include "etherShield.h"
#include "ETHER_28J60.h"
// Include libraries for digital temperature sensor
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

static uint8_t mac[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24};   // this just needs to be unique for your network, 
                                                                // so unless you have more than one of these boards
                                                                // connected, you should be fine with this value.
                                                           
static uint8_t ip[4] = {192, 168, 0, 115};                       // the IP address for your board. Check your home hub
                                                                // to find an IP address not in use and pick that
                                                                // this or 10.0.0.15 are likely formats for an address
                                                                // that will work.
                                                                // my router gives IPs from 192.168.0.100-199

static uint16_t port = 80;                                      // Use port 80 - the standard for HTTP

ETHER_28J60 ethernet;
void setup() {
  ethernet.setup(mac, ip, port); // setup connection
  
  // Start up the library
  sensors.begin();
}

void loop() {
  float temp;
  sensors.requestTemperatures(); // Send the command to get temperatures
  temp = sensors.getTempCByIndex(0);
  if (ethernet.serviceRequest())
  {
    ethernet.print("<H1>Current temperature from sensor 0</H1><br>");
    ethernet.print(temp);
    ethernet.respond();
  }
  delay(100);
}

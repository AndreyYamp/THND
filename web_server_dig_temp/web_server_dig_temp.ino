#include "NTPTime.h"
// Include libraries for ethernet shield
#include <UIPEthernet.h>
#include "TembooSendMail.h"
#include "Temperature.h"

#define ANALOG_INPUT 0

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte ethernetMACAddress[] = ETHERNET_SHIELD_MAC;

IPAddress ip(192, 168, 0, 115);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

// Set the previous time
time_t prevTime_g;
// Form the message
String message_g;

// Prints current time in a format: DD.MM.YYYY  HH:mm:SS
String currentTimeLong()
{
	String result = "Unknown";
	if (timeStatus() != timeNotSet){
		String currentDate = String("Date and time: ") + String(day()) + "." + String(month()) + "." + String(year());
		String minutes;
		String seconds;
		if (minute() < 10){
			minutes = "0" + String(minute());
		}
		else{
			minutes = String(minute());
		}

		if (second() < 10){
			seconds = "0" + String(second());
		}
		else{
			seconds = String(second());
		}

		String currentTime = String(hour()) + ":" + minutes + ":" + seconds;

		result = currentDate + "  " + currentTime;
	}
	return result;
}

// Function checks if 30 mins passed since the last Email notification
// If not, do not spam me please. 
boolean timeToSendEmail(time_t *prevTime)
{
	boolean result = false;
	if (timeStatus() != timeNotSet) {
		time_t diff = now() - *prevTime;
		Serial.println("diff: " + String(long(diff)));
		Serial.println("prev: " + String(long(*prevTime)));
		Serial.println("now: " + String(long(now())));
		if (*prevTime == 0 || diff >= 1800) { //check if it is first notification or 30 mins left after last notification
			*prevTime = now();
			result = true;
		}
	}
	else {
		result = false;
	}
	return result;
}

void setup() {
	// Open serial communications:
	Serial.begin(9600);
	// start the Ethernet connection and the server:
	Ethernet.begin(ethernetMACAddress, ip);
	server.begin();
	Serial.print("server is at ");
	Serial.println(Ethernet.localIP());
	// Get time
	Udp.begin(localPort);
	Serial.println("waiting for sync");
	setSyncProvider(getNtpTime);
	prevTime_g = 0;
	message_g = "";
}

void loop() {
	double temp;
	int analogVal;

	// listen for incoming clients
	EthernetClient client = server.available();
	if (client) {
		Serial.println("new client");
		// an http request ends with a blank line
		boolean currentLineIsBlank = true;
		while (client.connected()) {
			if (client.available()) {
				char c = client.read();
				Serial.write(c);
				// if you've gotten to the end of the line (received a newline
				// character) and the line is blank, the http request has ended,
				// so you can send a reply
				if (c == '\n' && currentLineIsBlank) {
					// send a standard http response header
					client.println("HTTP/1.1 200 OK");
					client.println("Content-Type: text/html");
					client.println("Connection: close");  // the connection will be closed after completion of the response
					client.println("Refresh: 30");  // refresh the page automatically every 30 sec
					client.println();
					client.println("<!DOCTYPE HTML>");
					client.println("<html>");
					// output the value of analog temperature sensor
					analogVal = analogRead(ANALOG_INPUT);
					temp = calcTemp(analogVal);
					client.print(currentTimeLong()); // print current date and time on a page
					client.println("<br />");
					client.print("Current temperature: "); // print current temperature on a page
					client.print(temp);
					if (checkTemp(temp, &message_g) == true){
						if (timeToSendEmail(&prevTime_g) == true){
							message_g += "\n" + currentTimeLong();
							sendEmail(client, message_g);
							client.print("<br />");
							client.print("Notification sent"); // print notification status
						}
					}
					client.println("<br />");
					client.println("</html>");
					break;
				}
				if (c == '\n') {
					// you're starting a new line
					currentLineIsBlank = true;
				}
				else if (c != '\r') {
					// you've gotten a character on the current line
					currentLineIsBlank = false;
				}
			}
		}
		// give the web browser time to receive the data
		delay(1);
		// close the connection:
		client.stop();
		Serial.println("client disonnected");
	}
}

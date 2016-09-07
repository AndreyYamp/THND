#include <Temboo.h>

// Include libraries for ethernet shield
#include <UIPEthernet.h>
// Include libraries for digital temperature sensor
#include <math.h>
// Include libraries for IoT (sending an email)
#include "TembooAccount.h" // Contains Temboo account information

#define UPPER_TEMP_LIMIT 40
#define LOWER_TEMP_LIMIT 1
#define ANALOG_INPUT 0

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte ethernetMACAddress[] = ETHERNET_SHIELD_MAC;

String message = "";
String senderEmail = "thnd.info@gmail.com";
  
IPAddress ip(192,168,0,115);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

// returns true if everything is ok, otherwise - false
boolean checkTemperature(double temp)
{
  boolean result = true;
  if (temp >= UPPER_TEMP_LIMIT || temp <= LOWER_TEMP_LIMIT){
    message = "Hello! This is notification from Arduino THND!\nTemperature limit exceeded: " + String(temp);
    result = false;
  }
  else{
    message = "";
    result = true;
  }
  return result;
}

double calcTemp(int rawVal){
  double tempAn;
  tempAn = log(10000.0*((1024.0/rawVal - 1))); 
  tempAn = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempAn * tempAn ))* tempAn );
  tempAn = tempAn - 273.15;

  return tempAn;
}

void sendEmail(EthernetClient client){

  TembooChoreo SendEmailChoreo(client);
  // Invoke the Temboo client
  SendEmailChoreo.begin();

  // Set Temboo account credentials
  SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
  SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);

  // Set Choreo inputs
  String FromAddressValue = senderEmail;
  SendEmailChoreo.addInput("FromAddress", FromAddressValue);
  String UsernameValue = senderEmail;
  SendEmailChoreo.addInput("Username", UsernameValue);
  String ToAddressValue = "andrey.yampolskiy@gmail.com,giesmo.home@gmail.com";
  SendEmailChoreo.addInput("ToAddress", ToAddressValue);
  String SubjectValue = "Warning!";
  SendEmailChoreo.addInput("Subject", SubjectValue);
  String PasswordValue = "yofkmsrxkpoklbth";
  SendEmailChoreo.addInput("Password", PasswordValue);
  String MessageBodyValue = message;
  SendEmailChoreo.addInput("MessageBody", MessageBodyValue);

  // Identify the Choreo to run
  SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");

  // Run the Choreo; when results are available, print them to serial
  SendEmailChoreo.run();

//  while(SendEmailChoreo.available()) {
//    char c = SendEmailChoreo.read();
//     Serial.print(c);
//  }
  SendEmailChoreo.close();
}

void setup() {
  // Open serial communications:
  Serial.begin(9600);
  // start the Ethernet connection and the server:
  Ethernet.begin(ethernetMACAddress, ip);
  server.begin();
//  Serial.print("server is at ");
//  Serial.println(Ethernet.localIP());
}

void loop() {
  double temp;
  int analogVal;

  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
//    Serial.println("new client");
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
          boolean tempCheck = checkTemperature(temp);
          client.print("Current temperature: ");
          client.print(temp);
          if (tempCheck == false){
            sendEmail(client);
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
//    Serial.println("client disonnected");
  }
}

/*
###############################################################################
# This file is responsible for email notification.
###############################################################################
*/

#include "TembooSendMail.h"

String senderEmail = "thnd.info@gmail.com";
String recipients = "recipient's emails";
String password = "password";

/*
###############################################################################
# sendEmail function:
# Uses Temboo library Google/Gmail/SendEmail to send notifications via Gmail
#
# INPUT: client: EthernetClient - ethernet client
# OUTPUT: void
###############################################################################
*/

void sendEmail(EthernetClient client, String message){

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
  String ToAddressValue = recipients;
  SendEmailChoreo.addInput("ToAddress", ToAddressValue);
  String SubjectValue = "Warning!";
  SendEmailChoreo.addInput("Subject", SubjectValue);
  String PasswordValue = password;
  SendEmailChoreo.addInput("Password", PasswordValue);
  String MessageBodyValue = message;
  SendEmailChoreo.addInput("MessageBody", MessageBodyValue);

  // Identify the Choreo to run
  SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");

  // Run the Choreo; when results are available, print them to serial
  SendEmailChoreo.run();

  while(SendEmailChoreo.available()) {
    char c = SendEmailChoreo.read();
     Serial.print(c);
  }
  SendEmailChoreo.close();
}

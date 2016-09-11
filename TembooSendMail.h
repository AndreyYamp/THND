#ifndef TEMBOO_SEND_MAIL_H_
#define TEMBOO_SEND_MAIL_H_

// Include libraries for IoT (sending an email)
#include "TembooAccount.h" // Contains Temboo account information
#include <Temboo.h>
#include <UIPEthernet.h>

void sendEmail(EthernetClient client, String message);

#endif

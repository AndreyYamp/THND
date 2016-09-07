#include "UIPServer_stub.h"

class UIPEthernetClass
{
public:
	UIPEthernetClass();

	boolean bBeginFuncCalled_g = false;
	const uint8_t* mac = "";

	void begin(const uint8_t* mac, IPAddress ip);
};

extern UIPEthernetClass UIPEthernet;
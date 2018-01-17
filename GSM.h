#pragma once

#include "gsmTerminal.h"
#include "SMS.h"
#include "gprs.h"

class GSM
{
public:
	GSM(int portNum, int baudRate);
	~GSM() { }
	SMS	sms;
	//GPRS gprs;
	bool getFailInitStatus() { return failInitStatus; }
private:
	SerialGate com_port;
	bool failInitStatus = false;
};



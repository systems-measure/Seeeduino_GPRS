#pragma once

#include "gsmTerminal.h"
#include "SMS.h"

class GSM
{
public:
	GSM(int portNum, int baudRate);
	~GSM() { }
	SMS	sms;
	bool getFailInitStatus() { return failInitStatus; }
private:
	SerialGate com_port;
	bool failInitStatus = false;
};



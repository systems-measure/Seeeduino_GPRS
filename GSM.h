#pragma once

#include "gsmTerminal.h"
#include "SMS.h"

class GSM
{
public:
	GSM(int portNum, int baudRate);
	~GSM() { }
	SMS	sms;
private:
	GSM_Terminal *gsmTerminal;
	
};



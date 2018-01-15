#pragma once

#include "gsmTerminal.h"
#include "SMS.h"

class GSM
{
public:
	GSM(int portNum, int baudRate);
	~GSM() { }
	SMS	sms;
	inline int getFailStatus() { return fail; }
private:
	GSM_Terminal *gsmTerminal;
	int fail = 0;
};



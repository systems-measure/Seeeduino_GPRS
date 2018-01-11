#pragma once
#include "gsmTerminal.h"
class SMS
{
public:
	SMS(GSM_Terminal *terminal);
	~SMS();
	
	int send(char *number, char *data);
	int read(int messageIndex, char *message, int length);

	inline int getFailStatus() { return fail; }
private:
	int fail = 0;

	GSM_Terminal *gsmTerminal;
};


#pragma once
#include "gsmTerminal.h"

#define GSM_TERMINAL_MAX_SMS_INDEX		35
class SMS
{
public:
	SMS(GSM_Terminal *terminal) { initSms(terminal); };
	SMS() {} 

	
	int send(char *number, char *data);
	int read(int messageIndex, char *message, int length);
	int readAndDelete(char *message, int length);
	int sendAndReadAnswer(char *number, char *request, char *responseBuf, int lenRespBuf, int tries = 100);
	int deleteMessage(int messageIndex);
	int deleteAllMessage();

	inline int getFailStatus() { return fail; }
	inline void setTerminal(GSM_Terminal *terminal) { initSms(terminal); }
private:
	void initSms(GSM_Terminal *terminal);

	int fail = 0;
	GSM_Terminal *gsmTerminal;
};


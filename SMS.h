#pragma once
#include "gsmTerminal.h"

#define GSM_TERMINAL_MAX_SMS_INDEX		35
class SMS: public GSM_Terminal
{
public:
	SMS(SerialGate *com) : GSM_Terminal(com) { initSms(); }
	SMS() {} 
	
	int send(char *number,const char *data);
	int read(int messageIndex, char *message, int length);
	int readAndDelete(char *message, int length);
	int sendAndReadAnswer(char *number, char *request, char *responseBuf, int lenRespBuf, int tries = 100);
	int deleteMessage(int messageIndex);
	int deleteAllMessage();

	inline int setComPort(SerialGate *com) { GSM_Terminal::setComPort(com); return initSms(); }
private:
	int initSms();

	void consoleMessage(std::string text)
	{
		if (consoleOutput) { std::cout << "GSM Terminal SMS: ";  std::cout << text; }
	}
	
};


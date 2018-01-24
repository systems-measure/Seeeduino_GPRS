#pragma once
#include "gsmTerminal.h"

#define GSM_TERMINAL_MAX_CSD_INDEX		35
class CSD: public GSM_Terminal
{
public:
	CSD(SerialGate *com) : GSM_Terminal(com) { init(); }
	CSD() {} 
	int Call(std::string number);
	int HangUp();

	

	inline int setComPort(SerialGate *com) { GSM_Terminal::setComPort(com); return init(); }
private:
	int init() { return sendCmdAndWaitForResp("AT&D2\r\n", "OK"); }
	void consoleMessage(std::string text)
	{
		if (consoleOutput) { std::cout << "GSM Terminal CSD: ";  std::cout << text; }
	}
	
};


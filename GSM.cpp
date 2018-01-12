#include "GSM.h"

GSM::GSM(int portNum, int baudRate)
{
	gsmTerminal = new GSM_Terminal(portNum, baudRate);
	sms.setTerminal(gsmTerminal);

}
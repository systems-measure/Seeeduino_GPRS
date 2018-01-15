#include "GSM.h"

GSM::GSM(int portNum, int baudRate)
{
	com_port.Open(portNum, baudRate);
	int ret = sms.setComPort(&com_port);
	if (ret < 0) failInitStatus = true;
}
#include "GSM.h"

GSM::GSM(int portNum, int baudRate)
{
	int ret = 0;
	com_port.Open(portNum, baudRate);

	ret += sms.setComPort(&com_port);
	//ret += gprs.setComPort(&com_port);

	if (ret < 0) failInitStatus = true;
}
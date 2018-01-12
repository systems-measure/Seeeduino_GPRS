#include "SMS.h"



void SMS::initSms(GSM_Terminal *terminal)
{
	if (terminal == NULL) return;
	gsmTerminal = terminal;

	if (0 != gsmTerminal->sendCmdAndWaitForResp("AT+CMGF=1\r\n", "OK", GSM_RESP_DEFAULT_TIMEOUT))
	{ // Set message mode to ASCII
		fail = -1;
	}

	if (deleteAllMessage() < 0) fail = -1;
}

int SMS::send(char *number, char *data)
{
	if (gsmTerminal == NULL) return -10;

	char cmd[32];


	snprintf(cmd, sizeof(cmd), "AT+CMGS=%s\r\n", number);
	gsmTerminal->sendCmd(cmd);
	Sleep(500); //wait ack 

	gsmTerminal->sendCmd(data);
	gsmTerminal->sendCmd("\x1A"); //end Mark
	Sleep(100);
	if (0 != gsmTerminal->sendCmdAndWaitForResp("\r\n", "+CMGS: ", "OK", 50000))
	{
		return -1;
	}
	return 0;
}

/*
	@retval size of readed string on seccess
			-1 of error
*/
int SMS::read(int messageIndex, char *message, int length)
{
	if (gsmTerminal == NULL) return -10;
	char cmd[16];

	memset(message, 0, length);

	sprintf(cmd, "AT+CMGR=%d\r\n", messageIndex);
	gsmTerminal->sendCmd(cmd);

	int ret = gsmTerminal->getFieldFromAnswer("\r\n", "\r\n", message, length, 2, 30000);
	return (ret >= 0) ? ret: -1;
}

int SMS::readAndDelete(char *message, int length)
{
	int ret = read(1, message, length);
	if (ret <= 0) { return ret; }
	deleteMessage(1);
	return ret;
}

int SMS::deleteMessage(int messageIndex)
{
	if (gsmTerminal == NULL) return -10;
	char cmd[16];
	snprintf(cmd,sizeof(cmd),"AT+CMGD=%d\r\n", messageIndex);
	return gsmTerminal->sendCmdAndWaitForResp(cmd, "OK");
	
}

int SMS::deleteAllMessage()
{
	for (int i = 1; i < GSM_TERMINAL_MAX_SMS_INDEX; i++)
	{
		if (deleteMessage(i) < 0) return -1;
	}
	return 0;
}

int SMS::sendAndReadAnswer(char *number, char *request, char *responseBuf, int lenRespBuf, int tries)
{
	int ret;

	ret = send(number, request);
	if (ret < 0) return -1;

	ret = 0;
	while ((ret == 0) && (tries > 0))
	{
		Sleep(1000);
		tries--;
		ret = readAndDelete(responseBuf, lenRespBuf);
		if (ret < 0) return -2;
	}
	if (tries <= 0) return -3;

	return ret;
}


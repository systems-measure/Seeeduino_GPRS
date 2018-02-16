#include "SMS.h"


SMS::SMS(int com_num, BAUD_RATE baud) : GSM_Terminal(com_num)
{
	com_port = new SerialGate;
	portOpen = com_port->Open(com_num, baud);
	initSms();
}
SMS::~SMS()
{
	if ((portOpen) && (com_port != nullptr))
	{
		
		com_port->Close();
	}
	
}
int SMS::initSms()
{
	consoleMessage("init ... ");
	if (0 != sendCmdAndWaitForResp("AT+CMGF=1\r\n", "OK", GSM_RESP_DEFAULT_TIMEOUT)) goto __error; // Set message mode to ASCII
	if (deleteAllMessage() < 0) goto __error;
	consoleMessage_Ok();
	return 0;

__error:
	{ 
		consoleMessage_Fail();
		return GSM_TERMINAL_ERROR_INIT;
	}

}

int SMS::send(char *number, const char *data)
{
	consoleMessage("send ... ");
	char cmd[32];

	snprintf(cmd, sizeof(cmd), "AT+CMGS=%s\r\n", number);
	GSM_Terminal::send(cmd);
	Sleep(500); //wait ack 
	
	GSM_Terminal::send(data);
	GSM_Terminal::send("\x1A"); //end Mark
	Sleep(100);
	if (0 != sendCmdAndWaitForResp("\r\n", "+CMGS: ", "OK", 50000))
	{
		consoleMessage("Fail! no answer");
		return GSM_TERMINAL_ERROR_NO_ANSWER;
	}
	Sleep(GSM_TERMINAL_SMS_SEND_DELAY * 1000);
	consoleMessage_Ok();
	return 0;
}

/*
	@retval size of readed string on seccess
			-1 of error
*/
int SMS::read(int messageIndex, char *message, int length)
{
	char cmd[16];
	memset(message, 0, length);

	sprintf_s(cmd, "AT+CMGR=%d\r\n", messageIndex);
	GSM_Terminal::send(cmd);

	int ret = getFieldFromAnswer("\r\n", "\r\n", message, length, 2, 30000);
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
	char cmd[16];
	snprintf(cmd,sizeof(cmd),"AT+CMGD=%d\r\n", messageIndex);
	return sendCmdAndWaitForResp(cmd, "OK");
	
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


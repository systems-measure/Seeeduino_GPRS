#include "SMS.h"



SMS::SMS(GSM_Terminal *terminal)
{
	gsmTerminal = terminal;
	if (0 != gsmTerminal->sendCmdAndWaitForResp("AT+CMGF=1\r\n", "OK", GSM_RESP_DEFAULT_TIMEOUT))
	{ // Set message mode to ASCII
		fail = -1;
	}

}


SMS::~SMS()
{
}


int SMS::send(char *number, char *data)
{
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

int SMS::read(int messageIndex, char *message, int length)
{
	int i = 0;
	char gprsBuffer[144]; // Buffer size for the SMS message
	char cmd[16];
	char *p, *s;


	sprintf(cmd, "AT+CMGR=%d\r\n", messageIndex);
	gsmTerminal->sendCmd(cmd);

	/*    cleanBuffer(gprsBuffer,144);
	readBuffer(gprsBuffer,144,DEFAULT_TIMEOUT);

	if(NULL != ( s = strstr(gprsBuffer,"+CMGR"))){
	if(NULL != ( s = strstr(gprsBuffer,"REC"))){  // Search the beginning of the SMS message
	p = s - 1;
	while((i < length-1)) {
	message[i++] = *(p++);
	}
	message[i] = '\0';
	}
	}*/
	return 0;
}


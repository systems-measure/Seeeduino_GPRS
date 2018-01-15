

#include "gsmTerminal.h"
#include <ctime>

GSM_Terminal::GSM_Terminal(int portNum, int baudRate)
{
	com_port.Open(portNum, baudRate);
	com_port.Clean();
	memset(buffer, '\0', sizeof(buffer));
};

int32_t GSM_Terminal::Receive(uint32_t timeout) {
	 

		int full_count = 0;
		int count_Rcv = 0;
		bool resp_flg = true;

		clock_t start_rcv = clock();
		short count_pack = -1;
		do {
			std::future<int> f = std::async(std::launch::async, &SerialGate::Recv, &com_port, (char*)(buffer + full_count), sizeof(buffer));
			count_Rcv = f.get();
		
			full_count += count_Rcv;
			if (full_count == 0) 
			{
				if ((clock() - start_rcv) > timeout * CLOCKS_PER_SEC) 
				{
					resp_flg = false;
				}
			}
			

			
		} 
		while ( (*(buffer + full_count-1) != (char)(0x0A)) && (*(buffer + full_count - 2) != (char)(0x0D)) && resp_flg);
		return full_count;
	
	
}


void GSM_Terminal::cleanBuffer()
{
	memset(buffer, '\0', sizeof(buffer));
}


int GSM_Terminal::sendCmd(const char* cmd)
{
	unsigned int len = strlen(cmd);
	if (len == NULL)
	{
		//consoleMessage("FAIL! \n", 1);
		return -1;
	}
	char *send_cmd = new char[len];
	memcpy((void*)send_cmd, (void*)cmd, len);
	com_port.Send(send_cmd, len);
}

int GSM_Terminal::sendCmdAndWaitForResp(char * cmd, const char * resp, unsigned timeout, uint32_t tries)
{
	unsigned tries_counter = 0;
	while (tries_counter < tries)
	{

		cleanBuffer();
		sendCmd(cmd);
		Receive(timeout);

		if (strstr(buffer, resp) != NULL)
		{
			return 0;
		}
		tries_counter++;
	}
	return -1;
}
int GSM_Terminal::sendCmdAndWaitForResp(char* cmd, const char *resp1,const char *resp2, uint32_t timeout, unsigned tries)
{
	unsigned tries_counter = 0;
	while (tries_counter < tries)
	{

		cleanBuffer();
		sendCmd(cmd);
		Receive(timeout);
		


		if ((strstr(buffer, resp1) != NULL) && (strstr(buffer, resp2) != NULL))
		{
			return 0;
		}
		tries_counter++;
	}
	return -1;
}

int GSM_Terminal::getFieldFromAnswer(char *startMark, char *endMark, char *outBuffer, int length, unsigned startMarkNumber,int timeout)
{
	char *dataStart, *dataEnd;
	int startMark_length = strlen(startMark);
	int endMark_length = strlen(endMark);
	int dataLength;

	cleanBuffer();

	Receive(timeout);
	//if (Receive(timeout) != 0) return -1;
	dataStart = buffer;

	
	while (startMarkNumber)
	{
		dataStart = strstr(dataStart, startMark);
		if (dataStart == NULL)
		{
			return -2;
		}
		dataStart += startMark_length;
		startMarkNumber--;
	}
	

	dataEnd = strstr(dataStart, endMark);
	if (dataEnd == NULL)
	{
		return -3;
	}
	dataLength = dataEnd - dataStart;

	if ((dataLength > length) || (dataLength < 0) 
		|| (dataEnd >(buffer + GSM_BUFFER_SIZE)) || (dataStart > (buffer + GSM_BUFFER_SIZE)))
	{
		return -4;
	}

		 
	memcpy(outBuffer, dataStart, dataLength);
	return dataLength;
}

//int GSM_Terminal::sendATTest(void)
//{
//    //int ret = sendCmdAndWaitForResp("AT\r\n","OK", DEFAULT_TIMEOUT);
//    //return ret;
//	return 0;
//}

//int GSM_Terminal::waitForResp(const char *resp, unsigned int timeout)
//{
//
//	//clock_t start_rcv = clock();
//	//while (1)
//	//{
//	//
//	//	readBuffer();
//	//	if (strstr(buffer, resp) != NULL)
//	//	{
//	//		return 0;
//	//	}
//
//	//	
//	//}
//	return -1;
//
//    /*while(1) 
//	{
//        if(serialGSM_Terminal.available()) 
//		{
//            char c = serialGSM_Terminal.read();
//            sum = (c==resp[sum]) ? sum+1 : 0;
//            if(sum == len)break;
//        }
//        timerEnd = millis();
//        if(timerEnd - timerStart > 1000 * timeout) 
//		{
//            return -1;
//        }
//    }
//
//    while(serialGSM_Terminal.available())
//	{
//        serialGSM_Terminal.read();
//    }
//
//    return 0;*/
//}

//void GSM_Terminal::sendEndMark(void)
//{
//    //serialGSM_Terminal.println((char)26);
//}





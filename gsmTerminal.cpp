/*
 * sim800.cpp
 * A library for SeeedStudio seeeduino GPRS shield 
 *
 * Copyright (c) 2013 seeed technology inc.
 * Author        :   lawliet zou
 * Create Time   :   Dec 2013
 * Change Log    :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "GsmTerminal.h"
#include <ctime>

SIM800::SIM800(int portNum, int baudRate)
{
	com_port.Open(portNum, baudRate);
	com_port.Clean();
	memset(buffer, '\0', sizeof(buffer));
};

int32_t SIM800::Receive(uint32_t timeout) {
	 
		/*std::future<int32_t> f = std::async(std::launch::async, &SerialGate::Recv, com_port_, (char*)rx_buff, count_rx);
		int32_t count_Rcv = f.get();*/
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
			

			/*if (full_count >= 3 && count_pack == -1) {
				count_pack = ((unsigned short)(*(rx_buff + 1) & 0x07) << 8 | (unsigned short)*(rx_buff + 2) & 0xFF) + 2;
			}*/
		} //while (/*(*(rx_buff + full_count - 1) != (char)(0x7E) || full_count < 10*/full_count != count_pack && resp_flg);
		while ( (*(buffer + full_count - 1) != (char)(0x0A)) && (*(buffer + full_count - 1) != (char)(0x0D)) && resp_flg);
		return full_count;
	
	
}


void SIM800::cleanBuffer()
{
	memset(buffer, '\0', sizeof(buffer));
}

void SIM800::sendCmd(char* cmd)
{
	unsigned int len = strlen(cmd);
	com_port.Send(cmd, len);
}

int SIM800::sendCmdAndWaitForResp(char* cmd, const char *resp, uint32_t timeout, unsigned tries)
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
int SIM800::sendCmdAndWaitForResp(char* cmd, const char *resp1,const char *resp2, uint32_t timeout, unsigned tries)
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
//int SIM800::sendATTest(void)
//{
//    //int ret = sendCmdAndWaitForResp("AT\r\n","OK", DEFAULT_TIMEOUT);
//    //return ret;
//	return 0;
//}

//int SIM800::waitForResp(const char *resp, unsigned int timeout)
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
//        if(serialSIM800.available()) 
//		{
//            char c = serialSIM800.read();
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
//    while(serialSIM800.available())
//	{
//        serialSIM800.read();
//    }
//
//    return 0;*/
//}

//void SIM800::sendEndMark(void)
//{
//    //serialSIM800.println((char)26);
//}





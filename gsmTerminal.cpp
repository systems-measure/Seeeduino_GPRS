#include <ctime>
#include <assert.h>
#include "gsmTerminal.h"

GSM_Terminal::GSM_Terminal(int com_number) : com_port(nullptr) {
    cons_log_ = new ConsoleLogger(com_number, "GSM_Term");
}
GSM_Terminal::GSM_Terminal(SerialGate *com) {
    assert(com != nullptr);

    com_port = com;
    cons_log_ = new ConsoleLogger(com->GetPort(), "GSM_Term");
}

GSM_Terminal::~GSM_Terminal() {
	if (cons_log_ != nullptr) {
		delete cons_log_;
		cons_log_ = nullptr;
	}
}

int32_t GSM_Terminal::Receive(uint32_t timeout, const char *resp1, const char *resp2)
{

		int full_count = 0;
		int count_Rcv = 0;
		bool resp_flg = true, fCheckTimeOut;

		clock_t start_rcv = clock();
		short count_pack = -1;
		do {
            fCheckTimeOut = false;
			std::future<int> f = std::async(std::launch::async, &SerialGate::Recv, com_port, (char*)(buffer + full_count), sizeof(buffer));
			count_Rcv = f.get();
		
			full_count += count_Rcv;
            // -- Check TimeOut
            if ((resp1) && (resp2)) fCheckTimeOut = !strstr(buffer, resp1) || !strstr(buffer, resp2);
            else if (resp1) fCheckTimeOut = !strstr(buffer, resp1);
            else fCheckTimeOut = (full_count == 0);

			if (fCheckTimeOut)
			{
                if ((clock() - start_rcv) > timeout * CLOCKS_PER_SEC)
                {
                    resp_flg = false;
                    cons_log_->LogOut("Response timeout.");
                }
			}
		} 
		while ((fCheckTimeOut || ((*(buffer + full_count-1) != (char)(0x0A)) && (*(buffer + full_count - 2) != (char)(0x0D)))) && resp_flg);

        cons_log_->LogRxASCII(buffer, full_count);

		return full_count;
}

void GSM_Terminal::cleanBuffer()
{
	memset(buffer, '\0', sizeof(buffer));
}


int GSM_Terminal::send(const char* cmd)
{
	unsigned int len = strlen(cmd);
	if (len == NULL)
	{
		return GSM_TERMINAL_ERROR_PARAM;
	}
	return send(cmd, len);
}
int GSM_Terminal::send(const char* data, int32_t size)
{
	char *send_data = new char[size];
	memset((void*)send_data, 0, size);
	memcpy((void*)send_data, (void*)data, size);

    cons_log_->LogTxASCII(send_data, size);
	int ret = com_port->Send(send_data, size);

	delete[] send_data;
	return ret;
}

int GSM_Terminal::sendCmdAndWaitForResp(const char * cmd, const char * resp, unsigned timeout, uint32_t tries, int tries_timeout)
{
	unsigned tries_counter = 0;
	while (tries_counter < tries)
	{

		cleanBuffer();
		send(cmd);
		Receive(timeout, resp);

		if (strstr(buffer, resp) != NULL)
		{
			return 0;
		}
		tries_counter++;
        Sleep(tries_timeout);
	}
	return GSM_TERMINAL_ERROR_NO_ANSWER;
}
int GSM_Terminal::sendCmdAndWaitForResp(const char* cmd, const char *resp1,const char *resp2, uint32_t timeout, unsigned tries, int tries_timeout)
{
	unsigned tries_counter = 0;
	while (tries_counter < tries)
	{

		cleanBuffer();
		send(cmd);
		Receive(timeout, resp1, resp2);
        if ((strstr(buffer, resp1) != NULL) && (strstr(buffer, resp2) != NULL))
        {
            return 0;
        }
        tries_counter++;
        Sleep(tries_timeout);
	}
	return GSM_TERMINAL_ERROR_NO_ANSWER;
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
			return GSM_TERMINAL_ERROR_PARAM;
		}
		dataStart += startMark_length;
		startMarkNumber--;
	}
	

	dataEnd = strstr(dataStart, endMark);
	if (dataEnd == NULL)
	{
		return GSM_TERMINAL_ERROR_PARAM;
	}  
	dataLength = dataEnd - dataStart;

	if ((dataLength > length) || (dataLength < 0) 
		|| (dataEnd >(buffer + GSM_BUFFER_SIZE)) || (dataStart > (buffer + GSM_BUFFER_SIZE)))
	{
		return GSM_TERMINAL_ERROR_INDEX_OUT_BUFFER;
	}
 
	memcpy(outBuffer, dataStart, dataLength);
	return dataLength;
}

#pragma once

#include "SerialGate.h"
#include <future>


#define GSM_RESP_DEFAULT_TIMEOUT    20000
#define GSM_BUFFER_SIZE				255

class GSM_Terminal
{
public:
	//GSM_Terminal(int portNum, int baudRate);
	GSM_Terminal(SerialGate *com) { com_port = com; }
	GSM_Terminal() {}

	/** send command to  GSM terminal 
	*  @param cmd command array which will be send to GSM terminal, need '/0' in the end
	*/
	int send(const char* cmd);
	int send(const char* data, int32_t size);

	/** send command to GSM terminal and wait for correct response
	*  @param  *cmd     command which will be send to GSM terminal
	*  @param  *resp   correct response which GSM module will return
	*  @param  *timeout    waiting seconds till timeout
	*  @returns
	*      0 on success
	*      -1 on error
	*/
	int sendCmdAndWaitForResp(const char* cmd, const char *resp, unsigned timeout = GSM_RESP_DEFAULT_TIMEOUT, uint32_t tries = 1);
	int sendCmdAndWaitForResp(const char* cmd, const char *resp1, const char *resp2, uint32_t timeout = GSM_RESP_DEFAULT_TIMEOUT, unsigned tries = 1);
	
	
	/* getting fields from answer. 
	*  @param  *startmark 	
	*  @param  *endMark    
	*  @param  *outBuffer   pointer on buffer for answer
	*  @param  length		size buffer
	*  @param  startMarkNumber   choice necessary entry substring  'startmark' in receive buffer
	*  @param timeout
	*  @returns
	*      size of string on seccess
	*       -1 timeout error
	*		-2 startmark not found
	*		-3 endMark   not found
	*		-4 buffer index error
	*/
	int getFieldFromAnswer(char *startMark, char *endMark, char *outBuffer, int length, unsigned startMarkNumber, int timeout = GSM_RESP_DEFAULT_TIMEOUT);
	
	virtual inline int setComPort(SerialGate *com) { com_port = com; return 0; }
protected:
	bool consoleOutput = true;
	virtual void consoleMessage(std::string text)
	{
		if (consoleOutput) { std::cout << "GSM Terminal : "; std::cout << text; }
	}
	void consoleMessage_Ok() { std::cout << "OK\n"; }
	void consoleMessage_Fail() { std::cout << "Fail\n"; }
private:
	void cleanBuffer();

	SerialGate *com_port;
	char buffer[GSM_BUFFER_SIZE];


	/** read from GSM terminal and save to buffer array
	*  @param  timeout 
	*  @returns
	*      0 on success
	*      -1 on error
	*/
	int32_t Receive(uint32_t timeout);
};

#define GSM_TERMINAL_ERROR						-1
#define GSM_TERMINAL_ERROR_INIT					-2
#define GSM_TERMINAL_ERROR_PARAM				-3
#define GSM_TERMINAL_ERROR_NO_ANSWER			-4
#define GSM_TERMINAL_ERROR_TIMEOUT				-5
#define GSM_TERMINAL_ERROR_INDEX_OUT_BUFFER		-6
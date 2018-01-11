#pragma once

#include "SerialGate.h"
#include <future>


#define GSM_RESP_DEFAULT_TIMEOUT     10000

class GSM_Terminal
{
public:
	GSM_Terminal(int portNum, int baudRate);
	inline ~GSM_Terminal() {/*com_port.Close();*/ };


	/** send command to  GSM terminal 
	*  @param cmd command array which will be send to GSM terminal, need '/0' in the end
	*/
	void sendCmd(char* cmd);

	/** send command to GSM terminal and wait for correct response
	*  @param  *cmd    AT command which will be send to GSM terminal
	*  @param  *resp   correct response which GSM module will return
	*  @param  *timeout    waiting seconds till timeout
	*  @returns
	*      0 on success
	*      -1 on error
	*/
	int sendCmdAndWaitForResp(char* cmd, const char *resp, unsigned timeout = GSM_RESP_DEFAULT_TIMEOUT, uint32_t tries = 1);
	int sendCmdAndWaitForResp(char* cmd, const char *resp1, const char *resp2, uint32_t timeout = GSM_RESP_DEFAULT_TIMEOUT, unsigned tries = 1);


protected:
	void cleanBuffer();

	SerialGate com_port;
	char buffer[255];

private:
	/** read from GSM terminal and save to buffer array
	*  @param  timeout 
	*  @returns
	*      0 on success
	*      -1 on error
	*/
	int32_t Receive(uint32_t timeout);
};

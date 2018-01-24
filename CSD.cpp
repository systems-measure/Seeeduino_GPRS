#include "CSD.h"
#include "SerialGate.h"


int CSD::Call(std::string number)
{
	com_port->SetLine(SerialGate::OUT_LINES_NAME::DTR, false);
	//std::string command = "ATD" + number + 
	//number.insert(number.begin, "ATD");
	//number.insert(number.end, "\r\n");
	number = "ATD" + number + "\r\n";
	return sendCmdAndWaitForResp(number.c_str(), "CONNECT");
}

int CSD::HangUp()
{
	sendCmd("ATH\r\n");
	com_port->SetLine(SerialGate::OUT_LINES_NAME::DTR, true);
	Sleep(50);
	com_port->SetLine(SerialGate::OUT_LINES_NAME::DTR, false);
	return sendCmdAndWaitForResp("", "ATH");
}
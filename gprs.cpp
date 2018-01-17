
#include "gprs.h"


int GPRS::setServiceProfile(gprsServiceParam_t &serviceParam)
{
	int ret = 0;
	gprsService = serviceParam;

	ret += sendParam("AT^SISS = ", gprsService.serviceProfile, "srvType", "Socket");
	ret += sendParam("AT^SISS = ", gprsService.serviceProfile, "conId", socket);
	ret += sendParam("AT^SISS = ", gprsService.serviceProfile, "tcpMR", "5");
	ret += sendParam("AT^SISS = ", gprsService.serviceProfile, "tcpOT", "20");
	switch (serviceParam.mode)
	{
	case gprsMode_CLIENT:
		ret += sendParam("AT^SISS = ", gprsService.serviceProfile, "address", "socktcp://" + gprsService.ip_adress + ":" + gprsService.port);
		break;
	case gprsMode_SERVER:
		ret += sendParam("AT^SISS = ", gprsService.serviceProfile, "address", "socktcp://listener:" + gprsService.port);
		break;
	default:
		break;
	}
	ret += sendCmdAndWaitForResp(("AT^SISO="+socket+"\r\n").c_str(), "OK");
	return ret;
}

int GPRS::sendParam(std::string cmd, std::string profileId, std::string param, std::string value)
{
	std::string sendCmd(cmd + profileId + ", " + param + ", \"" + value + "\"" + "\r\n");
	return sendCmdAndWaitForResp(sendCmd.c_str(), "OK");
}
int GPRS::setSetupPorfile(gprsParam_t &gprsParam)
{
	int ret = 0;
	socket = gprsParam.socket;
	socketClose();

	std::string conType;
	switch (gprsParam.conType)
	{
	case gprsConnectionType_CSD: conType = "CSD";
		break;
	case gprsConnectionType_GPRS0: conType = "GPRS0";
		break;
	case gprsConnectionType_NONE: conType = "none";
		break;
	default: break;
	}

	ret += sendParam("AT^SICS = ", gprsParam.socket, "conType", conType);
	ret += sendParam("AT^SICS = ", gprsParam.socket, "apn", gprsParam.apn);
	ret += sendParam("AT^SICS = ", gprsParam.socket, "user", gprsParam.userName);
	ret += sendParam("AT^SICS = ", gprsParam.socket, "passwd", gprsParam.password);
	
	return ret;
}
int GPRS::initGPRS(gprsParam_t &gprsParam, gprsServiceParam_t &serviceParam)
{
	int ret = 0;
	consoleMessage("init ... ");
	ret = setSetupPorfile(gprsParam);
	ret = setServiceProfile(serviceParam);


	if (ret < 0)
	{
		consoleMessage_Fail();
		return -1;
	}
	consoleMessage_Ok();
	return 0;
}

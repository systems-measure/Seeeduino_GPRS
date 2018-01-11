///*
// * gprs.cpp
// * A library for SeeedStudio seeeduino GPRS shield
// *
// * Copyright (c) 2013 seeed technology inc.
// * Author        :   lawliet zou
// * Create Time   :   Dec 2013
// * Change Log    :   2017-05-31 GPRS::readSMS modified to support SMS international
//                     increment the buffer to read the whole message 
//                     Jose Zorrilla (jzorrilla@x-red.com)
// *
// * The MIT License (MIT)
// *
// * Permission is hereby granted, free of charge, to any person obtaining a copy
// * of this software and associated documentation files (the "Software"), to deal
// * in the Software without restriction, including without limitation the rights
// * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// * copies of the Software, and to permit persons to whom the Software is
// * furnished to do so, subject to the following conditions:
// *
// * The above copyright notice and this permission notice shall be included in
// * all copies or substantial portions of the Software.
// *
// * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// * THE SOFTWARE.
// */
//
//#include <stdio.h>
//#include "gprs.h"
//
//int GPRS::init(void)
//{
//
//    if(sendCmdAndWaitForResp("AT\r\n","OK\r\n",DEFAULT_TIMEOUT*3, 3))
//	{
//		return -1;
//    }
//    if(sendCmdAndWaitForResp("AT+CFUN=1\r\n","OK\r\n",DEFAULT_TIMEOUT*3))
//	{
//		return -1;
//    }
//    if(checkSIMStatus())
//	{
//		return -1;
//    }
//	if (smsSetASKIImode())
//	{
//		return -1;
//	}
//    return 0;
//
//}
//
//bool GPRS::join(const char *apn, const char *userName, const char *passWord)
//{
////    char cmd[64];
////    char ipAddr[32];
////    char gprsBuffer[32];
////   
////    //Select multiple connection
////    //sim900_check_with_cmd("AT+CIPMUX=1\r\n","OK",DEFAULT_TIMEOUT,CMD);
////      
////    cleanBuffer(ipAddr,32);
////    sendCmd("AT+CIFSR\r\n");    
////    readBuffer(ipAddr,32,2);
////    
////    // If no IP address feedback than bring up wireless 
////    if( NULL != strstr(ipAddr, "ERROR") )
////    {
////        if( 0 != sendCmdAndWaitForResp("AT+CSTT?\r\n", apn, DEFAULT_TIMEOUT) )
////        {
////            sendCmd("AT+CSTT=\"");
////            sendCmd(apn);
////            sendCmd("\",\"");
////            sendCmd(userName);
////            sendCmd("\",\"");
////            sendCmd(passWord);        
////            sendCmdAndWaitForResp("\"\r\n","OK\r\n",DEFAULT_TIMEOUT*3);
////        }
////        
////        //Brings up wireless connection
////        sendCmd("AT+CIICR\r\n");
////         
////        //Get local IP address
////        cleanBuffer(ipAddr,32);
////        sendCmd("AT+CIFSR\r\n");
////        readBuffer(ipAddr,32,2);        
////    }          
////#if 0    
////    Serial.print("ipAddr: ");
////    Serial.println(ipAddr);
////#endif
////
////    if(NULL != strstr(ipAddr,"AT+CIFSR")) {        
////        _ip = str_to_ip(ipAddr+11);
////        if(_ip != 0) {
////            return true;
////        }
////    }
////    return false;
//
//	return false;
//}
//
//uint32_t GPRS::str_to_ip(const char* str)
//{
//    uint32_t ip = 0;
//    char *p = (char*)str;
//    
//    for(int i = 0; i < 4; i++) {
//        ip |= atoi(p);
//        p = strchr(p, '.');
//        if (p == NULL) {
//            break;
//        }
//        if(i < 3) ip <<= 8;
//        p++;
//    }
//    return ip;
//}
//
////HACERR lo de la IP gasta muuuucho espacio (ver .h y todo esto)
//char* GPRS::getIPAddress()
//{
//    uint8_t a = (_ip>>24)&0xff;
//    uint8_t b = (_ip>>16)&0xff;
//    uint8_t c = (_ip>>8)&0xff;
//    uint8_t d = _ip&0xff;
//
//    snprintf(ip_string, sizeof(ip_string), "%d.%d.%d.%d", a,b,c,d);
//    return ip_string;
//}
//
//int GPRS::checkSIMStatus(void)
//{
//    return sendCmdAndWaitForResp("AT+CPIN?\r\n", "+CPIN: READY", DEFAULT_TIMEOUT, 3);
//}
//
//int GPRS::networkCheck(void)
//{
//    //delay(1000);
//    //if(0 != sendCmdAndWaitForResp("AT+CGREG?\r\n","+CGREG: 0,1",DEFAULT_TIMEOUT*3)) {
//    //    ERROR("ERROR:CGREG");
//    //    return -1;
//    //}
//    //delay(1000);
//    //if(0 != sendCmdAndWaitForResp("AT+CGATT?\r\n","+CGATT: 1",DEFAULT_TIMEOUT)) {
//    //    ERROR("ERROR:CGATT");
//    //    return -1;
//    //}
//    return 0;
//}
//
//
//int GPRS::deleteSMS(int index)
//{
//    char cmd[16];
//    snprintf(cmd,sizeof(cmd),"AT+CMGD=%d\r\n",index);
//    sendCmd(cmd);
//    return 0;
//}
//
//int GPRS::callUp(char *number)
//{
//    //char cmd[24];
//    //if(0 != sendCmdAndWaitForResp("AT+COLP=1\r\n","OK",5)) {
//    //    ERROR("COLP");
//    //    return -1;
//    //}
//    //delay(1000);
//    //sprintf(cmd,"\r\nATD%s;\r\n", number);
//    //serialSIM800.write(cmd);
//    return 0;
//}
//
////int GPRS::answer(void)
////{
////    serialSIM800.write("ATA\r\n");
////    return 0;
////}
//
//int GPRS::connectTCP(const char *ip, int port)
//{
//    char cipstart[50];
//    sprintf(cipstart, "AT+CIPSTART=\"TCP\",\"%s\",\"%d\"\r\n", ip, port);
//    if(0 != sendCmdAndWaitForResp(cipstart, "CONNECT OK", 2*DEFAULT_TIMEOUT)) {// connect tcp
//        ERROR("ERROR:CIPSTART");
//        return -1;
//    }
//
//    return 0;
//}
//int GPRS::sendTCPData(char *data)
//{
//    char cmd[32];
//    int len = strlen(data); 
//    snprintf(cmd,sizeof(cmd),"AT+CIPSEND=%d\r\n",len);
//    if(0 != sendCmdAndWaitForResp(cmd,">",2*DEFAULT_TIMEOUT)) {
//        ERROR("ERROR:CIPSEND");
//        return -1;
//    }
//        
//    if(0 != sendCmdAndWaitForResp(data,"SEND OK",2*DEFAULT_TIMEOUT)) {
//        ERROR("ERROR:SendTCPData");
//        return -1;
//    }     
//    return 0;
//}
//
//int GPRS::closeTCP(void)
//{
//    sendCmd("AT+CIPCLOSE\r\n");
//    return 0;
//}
//
//int GPRS::shutTCP(void)
//{
//    sendCmd("AT+CIPSHUT\r\n");
//    return 0;
//}
/*
* gprs.h 
* A library for SeeedStudio seeeduino GPRS shield 
*  
* Copyright (c) 2013 seeed technology inc. 
* Author  		: 	lawliet.zou(lawliet.zou@gmail.com)
* Create Time	: 	Dec 23, 2013 
* Change Log 	: 
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

#ifndef __GPRS_H__
#define __GPRS_H__

#include "sim800.h"

/** GPRS class.
 *	used to realize GPRS communication
 */ 
#define MESSAGE_LENGTH  20
 
class GPRS: public SIM800
{
public:
    /** Create GPRS instance
     *  @param number default phone number during mobile communication
     */
    GPRS(int tx, int rx, int power, int baudRate, const char* number):SIM800(tx,rx,power,baudRate){
        phoneNumber = number;
    };
	
    /** initialize GPRS module including SIM card check & signal strength
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int init(void);

    /** check SIM card' Status
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int checkSIMStatus(void);

    /** check network is OK or not
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int networkCheck(void);

    /** send text SMS
     *  @param  *number	phone number which SMS will be send to
     *  @param  *data   message that will be send to
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int sendSMS(char* number, char* data);

    /** read SMS if getting a SMS message
     *  @param  buffer	buffer that get from GPRS module(when getting a SMS, GPRS module will return a buffer array)
     *  @param  message	buffer used to get SMS message
     *  @param  check  	whether to check phone number(we may only want to read SMS from specified phone number)
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int readSMS(int messageIndex, char *message, int length);

    /** delete SMS message on SIM card
     *  @param  index	the index number which SMS message will be delete
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int deleteSMS(int index);

    /** call someone
     *  @param  number	the phone number which you want to call
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int callUp(char* number);

    /** auto answer if coming a call
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int answer(void);
	
    /** build TCP connect
     *  @param  ip	ip address which will connect to
     *  @param  port	TCP server' port number
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int connectTCP(const char* ip, int port);

    /** send data to TCP server
     *  @param  data	data that will be send to TCP server
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int sendTCPData(char* data);

    /** close TCP connection
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int closeTCP(void);

    /** close TCP service
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int shutTCP(void);
	
private:
	const char* phoneNumber;
};
#endif
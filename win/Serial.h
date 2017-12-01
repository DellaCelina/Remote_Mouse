#pragma once

#define UNICODE

#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <string>
#include "Exception.h"

#define HANDLEERR 0
#define DCBERR 1
#define TIMEOUTSERR 2

using namespace std;

class Serial {
private:
	HANDLE hSerial;
	LPCWSTR port;

	//DCB Setting
	DCB dcbSerial;
	UINT baudRate;
	UINT byteSize;
	UINT stopBit;
	UINT parity;

	//Timeout Setting
	COMMTIMEOUTS timeouts;

public:
	Serial(LPCWSTR port, UINT baudRate, UINT byteSize, UINT stopBit, UINT parity)
		:port(port), baudRate(baudRate), byteSize(byteSize), stopBit(stopBit), parity(parity) {

		//Serial Port handle
		if ((hSerial = CreateFile(port,
			GENERIC_READ | GENERIC_WRITE,
			0,
			0,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			0)) == INVALID_HANDLE_VALUE) throw Exception("Create File Error\n", HANDLEERR);

		//DCB setting
		dcbSerial.DCBlength = sizeof(dcbSerial);
		if (!GetCommState(hSerial, &dcbSerial)) throw Exception("GetCommStateError\n", DCBERR);
		
		dcbSerial.BaudRate = baudRate;
		dcbSerial.ByteSize = byteSize;
		dcbSerial.StopBits = stopBit;
		dcbSerial.Parity = parity;

		if (!SetCommState(hSerial, &dcbSerial)) throw Exception("SetCommStateError\n", DCBERR);

		//Timeouts setting
		if (!SetCommTimeouts(hSerial, &timeouts)) throw Exception("SetCommTimeoutsError\n", TIMEOUTSERR);
		
	}
	~Serial() {
		if(hSerial != INVALID_HANDLE_VALUE)	CloseHandle(hSerial);
		delete[] port;
	} 

	//Port Read, Write
	bool Read(char* buf, UINT size);
	bool Write(char data);
};
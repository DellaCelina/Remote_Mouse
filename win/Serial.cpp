#include "Serial.h"

bool Serial::Read(char * buf, UINT size){
	DWORD byteTransferred = 0;
	if (ReadFile(hSerial, buf, size, &byteTransferred, 0)) {
		if (byteTransferred == size) return true;
	}

	return false;
}

bool Serial::Write(char data){
	DWORD byteWrite = 0;
	if (WriteFile(hSerial, &data, 1, &byteWrite, 0))
		return true;
	return false;
}

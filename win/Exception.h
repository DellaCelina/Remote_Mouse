#pragma once
#include <stdio.h>

class Exception {
private:
	const char* msg;
	const int errnum;
public:
	Exception(const char* msg, const int errnum) :msg(msg), errnum(errnum) {}
	~Exception() {};
	void print();
	bool operator==(Exception &a);
};
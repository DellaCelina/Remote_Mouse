#include "Exception.h"

void Exception::print(){
	printf("%s", msg);
}

bool Exception::operator==(Exception & a){
	return (*this).errnum == a.errnum;
}
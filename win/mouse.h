#pragma once
#include <math.h>
#define UARTSIZE 2

//upper byte ���� 3��Ʈ
#define MOUSEEVNETSHIFT 5
#define MOUSEMOVEX 1 << MOUSEEVNETSHIFT
#define MOUSEMOVEY 2 << MOUSEEVNETSHIFT
#define MOUSELEFTDOWN 3 << MOUSEEVNETSHIFT
#define MOUSELEFTUP 4 << MOUSEEVNETSHIFT
#define MOUSERIGHTDOWN 5 << MOUSEEVNETSHIFT
#define MOUSERIGHTUP 6 << MOUSEEVNETSHIFT
#define MOUSESTART 7 << MOUSEEVNETSHIFT

#define SIGN (1 << 12)

float mouse_sensitivity;	//���콺 �ΰ���
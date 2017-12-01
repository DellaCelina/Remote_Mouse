#include "mouse.h"

UINT8 INT6Pushed = 0;
UINT8 INT7Pushed = 0;

UINT8 rightPushed, leftPushed;

extern UINT8 chk_button6;
extern UINT8 chk_button7;
extern UINT8 chk_timer0;

UINT8 count = 0;
double curruntX, curruntY;

int main(){
	init_gpio();
	init_timer();
	init_adc();
	uart_init(UART_CH0,STOP_BIT1,PARITY_NONE,BAUD_9600,NO,NO);
	sei();
	while(1){	//loop for interrupt
		//check interruped
		if(chk_button6) button6();
		if(chk_button7) button7();
		if(chk_timer0) timer0();
	}
}

void button6(){
	if(!(PINE & 0x40) && INT6Pushed == 0) {
		INT6Pushed = 1;
		count = 0;	//init CLICK count
		TIMSK |= 0x02;	//TIMER, Externel INTERRUPT Set for polling
	}
	else if(PINE & 0x40 && INT6Pushed == 1){
		if(count < CLICKCOUNT) {
			uart_send16(MOUSELEFTDOWN);
			uart_send16(MOUSELEFTUP);
		}
		INT6Pushed = 0;
		TIMSK &= ~(0x02);
	}

	chk_button6 = 0;
}
void button7(){
	if(!(PINE & 0x80) && INT7Pushed == 0) {	//´­·¶À» °æ¿ì
		if(INT6Pushed == 0)	{
			uart_send16(MOUSERIGHTDOWN);
			rightPushed = 1;
		}
		else if(INT6Pushed == 1 && count >= CLICKCOUNT) {
			uart_send16(MOUSELEFTDOWN);
			leftPushed = 1;
		}
		INT7Pushed = 1;
	}
	else if(PINE & 0x80 && INT7Pushed == 1){ //¶¿ °æ¿ì
		if(rightPushed == 1) {
			uart_send16(MOUSERIGHTUP);
			rightPushed = 0;
		}
		else if(leftPushed == 1) {
			uart_send16(MOUSELEFTUP);
			rightPushed = 0;
		}
		INT7Pushed = 0;
	}

	chk_button7 = 0;
}

void timer0(){
	if(count < MAXCOUNT) count++;
		if(count == CLICKCOUNT){
			ADC_XYZ adc_xyz;

			getADC_XYZ(&adc_xyz);

			//set currunt tilt;
			curruntX = getXDegree(&adc_xyz);
			curruntY = getYDegree(&adc_xyz);

			//send start
			uart_send16(MOUSESTART);
		}
		else if(count > CLICKCOUNT){
			uart_send16(conversion(TILTX) | MOUSEMOVEX);
			uart_send16(conversion(TILTY) | MOUSEMOVEY);
		}
		TCNT0 = 0x00;

		chk_timer0 = 0;
}

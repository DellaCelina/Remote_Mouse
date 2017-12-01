#include "mouse.h"

UINT8 chat_value6 = 0;
UINT8 chat_value7 = 0;
UINT8 cnt_chat6 = INIT_CHAT;
UINT8 cnt_chat7 = INIT_CHAT;

extern UINT8 count;	//for check click or move when button6 switched

ISR(INT6_vect){
	chat_value6 = (PINE & 0x40) ? HIGH : LOW;
	cnt_chat6 = 0;
	TIMSK |= 0x80;
}

ISR(INT7_vect){
	chat_value7 = (PINE & 0x80) ? HIGH : LOW;
	cnt_chat7 = 0;
	TIMSK |= 0x80;
}

void init_gpio(){
	DDRF = 0x00;	//input set	for sensor
	DDRE = 0x00;	//input set	for button
	PORTE = 0xF0;	//for external button

	//interrupt
	EICRB = 0x01 | 0x04 | 0x10 | 0x40;
	EIMSK = 0x10 | 0x20 | 0x40 | 0x80;
}

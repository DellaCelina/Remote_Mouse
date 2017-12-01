#include "mouse.h"

UINT8 chk_timer0 = 0;

extern UINT8 chat_value6, chat_value7;
extern UINT8 cnt_chat6, cnt_chat7;

UINT8 chk_button6 = 0;
UINT8 chk_button7 = 0;

ISR(TIMER0_COMP_vect){	//timer for polling
	chk_timer0 = 1;
}

ISR(TIMER2_COMP_vect){	//for remove chattering in button
	TCNT2 = 0x00;
	if( cnt_chat6 == CHK_CHAT ) {
		UINT8 currunt_value = (PINE & 0x40) ? HIGH : LOW;
		if (currunt_value == chat_value6) chk_button6 = 1;	//현재 값과 interrunpt 당시을 비교하여 valid검사
	}

	if( cnt_chat7 == CHK_CHAT ) {
		UINT8 currunt_value = (PINE & 0x80) ? HIGH : LOW;
		if (currunt_value == chat_value7) chk_button7 = 1;	//현재 값과 interrunpt 당시을 비교하여 valid검사
	}

	if( !(cnt_chat6 > CHK_CHAT) ) cnt_chat6++;
	if( !(cnt_chat7 > CHK_CHAT) ) cnt_chat7++;
	if( (cnt_chat6 > CHK_CHAT) && (cnt_chat7 > CHK_CHAT) ) TIMSK &= ~(0x80);
}

void init_timer(){
	TCCR0 = 0x07;	//1024 prescale
	TCCR2 = 0x05;	//128 prescale
	TIMSK = 0x00;	//set by func to 0x02 for polling, 0x80 for chattering
	TCNT0 = 0x00;
	TCNT2 = 0x00;
	OCR0 = 156;		//100HZ	156
	OCR2 = 76;	//2.5ms
}

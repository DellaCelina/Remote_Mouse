#include "mouse.h"

typedef struct _baud_tbl_t { 
 UINT16 ubrr; 
 UINT8 UX2;
} baud_tbl_t;

baud_tbl_t mcu_baud_tbl[9] = {
 {  416,1}, /* baud =    4800 */ 
 {  103,0}, /* baud =    9600 */ 
 {   51,0}, /* baud =   19200 */ 
 {   25,0}, /* baud =   38400 */ 
 {   34,1}, /* baud =   57600 */ 
 {   16,1}, /* baud =  115200 */ 
 {    1,0}, /* baud =  500K   */
 {    0,0}, /* baud =  1M     */
 {    3,0}, /* buad =  250K   */
};

int uart_init(int ch,int stop,int parity, int baud, int rx_intr,int tx_intr)
{


 if( ch == UART_CH0 )
 {
  UCSR0A = (mcu_baud_tbl[baud].UX2 << U2X);
  UCSR0B = 0;
  if(parity == PARITY_NONE)
   UCSR0C = 0 << UPM10;
  else if(parity == PARITY_ODD)
   UCSR0C = 3 << UPM10;
  else // Even parity
   UCSR0C = 2 << UPM10;

  UCSR0C |= 3 << 1; // 8 bits default
  UBRR0H = mcu_baud_tbl[baud].ubrr >> 8;
  UBRR0L = mcu_baud_tbl[baud].ubrr & 0xff;
  if(rx_intr) UCSR0B |= (1 << RXCIE1);
  if(tx_intr) UCSR0B |= (1 << UDRIE);
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

  return 0;
 }
 else if( ch == UART_CH1 )
 {
  UCSR1A = (mcu_baud_tbl[baud].UX2 << U2X);
  UCSR1B = 0;
  if(parity == PARITY_NONE)
   UCSR1C = 0 << UPM10;
  else if(parity == PARITY_ODD)
   UCSR1C = 3 << UPM10;
  else // Even parity
   UCSR1C = 2 << UPM10;

  UCSR1C |= 3 << 1; // 8 bits default
  UBRR1H = mcu_baud_tbl[baud].ubrr >> 8;
  UBRR1L = mcu_baud_tbl[baud].ubrr & 0xff;
  if(rx_intr) UCSR1B |= (1 << RXCIE1);
  if(tx_intr) UCSR1B |= (1 << UDRIE);
  UCSR1B |= (1 << RXEN1) | (1 << TXEN1);
  return 0;
 }
 return -1;
}

UINT8 uart_receive8(void){
 while(!(UCSR0A & (1 << RXC))); 
 return UDR0 & 0xff;
}

void uart_send8(UINT8 data){
 while(!(UCSR0A & (1 << UDRE1))); 
 UDR0 = data;
}

void uart_send16(UINT16 data){
	UINT8 upper, low;
	upper = data>>8;
	low = data & 0xFF;
	uart_send8(upper);
	uart_send8(low);
}

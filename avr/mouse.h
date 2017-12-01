#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#define FCPU 16000000UL
#include <util/delay.h>

//Typedef for bits
typedef char INT8;
typedef int  INT16;
typedef long INT32;
typedef unsigned char UINT8 ;
typedef unsigned int  UINT16;
typedef unsigned long UINT32;

#define SYS_CLK 16000000

//UART Setting const
#define UART_RX_POLL 0
#define UART_RX_INT  1
#define UART_TX_POLL 0
#define UART_TX_BUFFER_INT  1

#define UART_CH0  0
#define UART_CH1  1
#define STOP_BIT1  0
#define STOP_BIT1_5 1
#define STOP_BIT2  2
#define PARITY_NONE 0
#define PARITY_ODD  1
#define PARITY_EVEN  2
#define BAUD_4800  0
#define BAUD_9600  1
#define BAUD_19200 2
#define BAUD_38400 3
#define BAUD_57600  4
#define BAUD_115200 5
#define BAUD_500K   6
#define BAUD_1M     7
#define BAUD_250K   8

//Bool Define
#define YES  1
#define NO  0
#define FAIL  -1
#define SUCCESS  0

//UART Send Data
#define MOUSEEVNETSHIFT 13
#define MOUSEMOVEX 1 << MOUSEEVNETSHIFT
#define MOUSEMOVEY 2 << MOUSEEVNETSHIFT
#define MOUSELEFTDOWN 3 << MOUSEEVNETSHIFT
#define MOUSELEFTUP 4 << MOUSEEVNETSHIFT
#define MOUSERIGHTDOWN 5 << MOUSEEVNETSHIFT
#define MOUSERIGHTUP 6 << MOUSEEVNETSHIFT
#define MOUSESTART 7 << MOUSEEVNETSHIFT

#define SIGN 1 << 12

//ADC DIR
#define DIRX 1
#define DIRY 2
#define DIRZ 3

//conversion const
#define TILTX 1
#define TILTY 2

//SENSOR Setting
#define SENSOR_MIN 260
#define SENSOR_MAX 400
#define SENSOR_SUB (SENSOR_MIN + (SENSOR_MAX-SENSOR_MIN) / 2)
#define SENSOR_VALID 100

//ANGLE Mux rate
#define ANGLE_MUX 1024

//TIMER Click check count
#define MAXCOUNT 200
#define CLICKCOUNT 20

//For Mouse Dimension
#define ZDIMENSION 1
#define XDIMENSION 1
#define YDIMENSION 0

//For chattering
#define LOW 0x10
#define HIGH 0x20
#define CHK_CHAT 10
#define INIT_CHAT (CHK_CHAT+100)

//xyz struct
typedef struct xyz{
	double x;
	double y;
	double z;
} ADC_XYZ;

void button6();
void button7();
void timer0();

void init_gpio();
void init_timer();
int uart_init();
void init_adc();

UINT8 uart_receive8(void);
void uart_send8(UINT8);
void uart_send16(UINT16);

UINT16 read_adc(UINT8 input);
UINT16 avr_adc(UINT8 input);
UINT16 conversion(UINT8 set);

void getADC_XYZ(ADC_XYZ *xyz);
double getXDegree(ADC_XYZ *xyz);
double getYDegree(ADC_XYZ *xyz);

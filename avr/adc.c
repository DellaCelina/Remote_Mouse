#include "mouse.h"
#define ADMUX_INNER_V 0x40

extern double curruntX, curruntY;

void init_adc(){
	ADMUX = ADMUX_INNER_V;	//AVCC 전압 사용
	ADCSRA = 0x87;	//ADC enable, scale:128
}

UINT16 read_adc(UINT8 input){
	UINT16 tmp;
	ADMUX = input | ADMUX_INNER_V;	//내부전압 사용, input adc 선택
	ADCSRA |= 0x40;	//ADC 변환 시작
	while( (ADCSRA & 0x10) == 0);	//ADC 변환 완료까지 루프

	tmp = ADC;
	if(tmp >= SENSOR_MAX + SENSOR_VALID) tmp = 0;
	if(tmp <= SENSOR_MIN - SENSOR_VALID) tmp = 0; 
	return tmp;	//변환 레지스터값 리턴
}

void getADC_XYZ(ADC_XYZ *xyz){
	(*xyz).x = (double)read_adc(DIRX) - SENSOR_SUB;
	(*xyz).y = (double)read_adc(DIRY) - SENSOR_SUB;
	(*xyz).z = (double)read_adc(DIRZ) - SENSOR_SUB;

	//Dimension control
	#if ZDIMENSION == 1
	(*xyz).z *= -1;
	#endif

	#if XDIMENSION == 1
	(*xyz).x *= -1;
	#endif

	#if YDIMENSION == 1
	(*xyz).y *= -1;
	#endif
}

double getXDegree(ADC_XYZ *xyz){ return atan((*xyz).z / (*xyz).y) * ANGLE_MUX; }
double getYDegree(ADC_XYZ *xyz){ return atan((*xyz).x / (*xyz).y) * ANGLE_MUX; }


UINT16 avr_adc(UINT8 input){
	const UINT16 size = 32;
	UINT16 i, sum = 0;
	for(i = 0; i< size; i++){
		sum += read_adc(input);
	}
	return sum >> 5;
}

UINT16 conversion(UINT8 set){
	ADC_XYZ adc_xyz;	//sensor conversion to 0
	double tmp;		//tmp for sign

	const double radian180 = M_PI * ANGLE_MUX;

	//read adc from sensor
	getADC_XYZ(&adc_xyz);

	//TILT X Case
	if(set == TILTX){
		double tmp;
		tmp = getXDegree(&adc_xyz);	//각도로 변환

	//z에 따른 tmp와 currunt 비교 각도로 변환
		if(adc_xyz.y <= 0) tmp = tmp - curruntX;
		else {
			if(tmp < 0)	tmp = radian180 + tmp - curruntX;
			else tmp = -radian180 + tmp - curruntX;
		}

		//tmp overflow 조절
		if(tmp >= (double)(1<<12)) tmp = 0;	
		if(-tmp >= (double)(1<<12)) tmp = 0;

		return (tmp>=0) ? ((UINT16)tmp) : ((UINT16)(-tmp) | SIGN);	//SIGN비트와 결합후 return
	}

	//TILT Y Case
	else if(set == TILTY){
		tmp = getYDegree(&adc_xyz);	//각도로 변환

	//z에 따른 tmp와 currunt 비교 각도로 변환
		if(adc_xyz.y <= 0) tmp = tmp - curruntY;
		else {
			if(tmp < 0)	tmp = radian180 + tmp - curruntY;
			else tmp = -radian180 + tmp - curruntY;
		}

		//tmp overflow 조절
		if(tmp >= (double)(1<<13)) tmp = 0;	
		if(-tmp >= (double)(1<<13)) tmp = 0;

		return (tmp>=0) ? ((UINT16)tmp) : ((UINT16)(-tmp) | SIGN);	//SIGN비트와 결합후 return
	}
	else return FAIL;
}

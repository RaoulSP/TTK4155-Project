#include <avr/io.h>
#include "utils.h"
#define F_CPU 4915200
#include <util/delay.h>

/*ISR(INT2_vect)
{
	adcConversionRunning = 0;
}*/


uint8_t ADC_read(char channel)
{
	volatile char *ext_adc = (char *) 0x1200; 
	if (channel == 'x')
	{
		*ext_adc = 0b0100;
	}
	
	else if (channel == 'y')
	{
		*ext_adc = 0b0101;
	}
	
	else if (channel == 'l')
	{
		*ext_adc = 0b0110;
	}
	
	else if (channel == 'r')
	{
		*ext_adc = 0b0111;
	}
	while(test_bit(PORTE, PE0));
	_delay_us(5);
	return (uint8_t) *ext_adc;
}
#include <avr/io.h>
#include "utils.h"

/*ISR(INT2_vect)
{
	adcConversionRunning = 0;
}*/


void ADC_read(char channel)
{
	//adcConversionRunning = 1;
	volatile char *ext_adc = (char *) 0x1200; 
	if (channel == 'x')
	{
		*ext_adc = 4;
	}
	else if (channel == 'y')
	{
		*ext_adc = 5;
	}

	while(test_bit(PORTE, PE0));
	printf("%d",(int) *ext_adc);
}
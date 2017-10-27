#include <avr/io.h>
#include <avr/delay.h>
#include "adc.h"

void adc_init(){
	ADCSRA |= (1 << ADEN); //Enable
	//ADMUX |= ((0 << REFS1) | (1 << REFS0)); //Enable left align of returned data
	//ADCSRB for mux?
}
/*
ADCSRA //Status register?
ADSC //start conversion
ADIF //Interrupt flag
*/

uint16_t adc_read(){
	ADCSRA |= (1 << ADSC);
	while (test_bit(ADCSRA, ADSC)){
	}
	return ADC;
}
#include <avr/io.h>
#include <avr/delay.h>
#include "adc.h"
#include "utils.h"

void adc_init(){
	ADCSRA |= (1 << ADEN); //Enable
	ADMUX |= ((0 << REFS1) | (0 << REFS0)); //00 = AREF, internal VREF turned off; 01 = AVCC with external capacitor at AREF pin
	//ADMUX |= (1 << ADLAR); //Left shift ADC
	//ADCSRB for mux?
}
/*
ADCSRA //Status register?
ADSC //start conversion
ADIF //Interrupt flag
*/

int16_t adc_read(){
	ADCSRA |= (1 << ADSC);
	while (test_bit(ADCSRA, ADSC)){
	}
	return ADC;
}
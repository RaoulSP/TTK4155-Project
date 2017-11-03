#include <avr/io.h>
#include <avr/delay.h>
#include "adc.h"
#include "utils.h"

void adc_init(){
	ADCSRA |= (1 << ADEN); //Enable
	ADMUX |= ((1 << REFS1) | (1 << REFS0)); //00 = AREF, internal VREF turned off; 01 = AVCC with external capacitor at AREF pin; 11 = 2.56V
	ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)); //div 128
	//ADMUX |= (1 << ADLAR); //Left shift ADC
	//ADCSRB for mux?
}

int16_t adc_read(){ //let this take channel as argument
	ADCSRA |= (1 << ADSC);
	while (test_bit(ADCSRA, ADSC)){
	}
	return ADC;
}


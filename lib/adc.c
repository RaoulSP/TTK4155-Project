#include <util/delay.h>
#include "adc.h"
/*ISR(INT2_vect)
{
	adcConversionRunning = 0;
}*/

#ifdef NODE_1
	void adc_init(){
	clear_bit(DDRE, PE0); //Set DDR to input for interrupt flag
}
	uint8_t adc_read(char channel){
		volatile char *ext_adc = (char *) 0x1400; 
		if (channel == 'x'){
			*ext_adc = 0b0100;
		}
	
		else if (channel == 'y'){
			*ext_adc = 0b0101;
		}
	
		else if (channel == 'l'){
			*ext_adc = 0b0110;
		}
	
		else if (channel == 'r'){
			*ext_adc = 0b0111;
		}
		while(test_bit(PINE, PE0)); //Wait for interrupt flag to signify completed conversion
		return (uint8_t) *ext_adc;
}
#else
	void adc_init(){ //ADC input pin is not specified. A0 default? otherwise set multiplexer
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
#endif

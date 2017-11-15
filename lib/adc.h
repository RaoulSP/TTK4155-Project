//#include <avr/interrupt.h>

#ifndef ADC_H_
#define ADC_H_

#include "settings.h"

#ifdef NODE_1
	void adc_init();
	uint8_t adc_read(char channel);
#else
	void adc_init();
	int16_t adc_read();
#endif


#endif /* ADC_H_ */
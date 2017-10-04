#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

#include <avr/interrupt.h>

#ifndef ADC_H_
#define ADC_H_

void adc_init();
uint8_t adc_read(char channel);

#endif /* ADC_H_ */
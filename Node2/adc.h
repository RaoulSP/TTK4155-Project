#ifndef ADC_H_
#define ADC_H_

#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

void adc_init();
uint16_t adc_read();

#endif /* ADC_H_ */
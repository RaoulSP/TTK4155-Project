#include <avr/io.h>
#include "touch.h"

void touch_init(){
	clear_bit(DDRB, PB0); //Set DDR to input for button
	clear_bit(DDRB, PB1); //Set DDR to input for button
}

Touch_positions touch_get_positions(){
		Touch_positions pos;
		
		pos.l = adc_read('l');
		pos.r = adc_read('r');
		
		pos.l = (100*pos.l)/255;
		pos.r = (100*pos.r)/255;
		
		pos.lb = !!test_bit(PINB, PB1);
		pos.rb = !!test_bit(PINB, PB0);
		
		return pos;
}

void touch_print(int all){
	if (all){
		printf("l = ");
		printf("%d  ", adc_read('l'));
		
		printf("r = ");
		printf("%d  ", adc_read('r'));
	}
	
	Touch_positions pos = touch_get_positions();
	
	printf("Left slider = ");
	printf("%d  ",pos.l);

	printf("Right slider = ");
	printf("%d  ", pos.r);
	
	printf("Left button= ");
	printf("%d  ", pos.lb);
	
	printf("Right button= ");
	printf("%d  ", pos.rb);
}
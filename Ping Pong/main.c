#define F_CPU 4915200

#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"
#include "adc.h"
#include "touch.h"
#include "joy.h"
#include "oled.h"
#include "sram.h"

/*
JTAG interface with the 168

Left to right
1,5,3,9

Vcc 4
GND 2
*/

//1 output, 0 input

int main(void)
{
	MCUCR |= (1<<SRE);		//Enable external memory
	SFIOR |= (1<<XMM2);
	
	//Initialization
	uart_init(9600);
	adc_init();
	oled_init();
	joy_init();
	touch_init();
	
	oled_print_string("New Ger",0,0,8,0);
	oled_print_string("High score",0,1,8,0);
	oled_print_string("Options",0,2,8,0);
	oled_refresh();
	oled_draw_pixel(50,800);
	oled_clear_pixel(1,2);
	oled_refresh();
	
	sram_test();
	
	while (1)
	{
		joy_print(0);
		touch_print(0);
		printf("\n");
		_delay_ms(10);
	}

}

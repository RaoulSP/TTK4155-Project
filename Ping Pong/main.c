#define F_CPU 4915200

#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"
#include "adc.h"
#include "touch.h"
#include "joy.h"
#include "oled.h"
#include "sram.h"
#include "menu2.h"

/*--TODO--
-Add a brightness function
-Add a interrupt for 60 Hz updates
*/

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
	oled_print_string("Please calibrate joystick!",0,0,8,0);
	oled_refresh();
	joy_init();
	touch_init();
	oled_clear_screen();
	oled_refresh();
	init_menus();
	
	while (1)
	{
		run_display();
	}

}

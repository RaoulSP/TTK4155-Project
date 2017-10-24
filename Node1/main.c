#define F_CPU 4915200
#define NODE_1 1

#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"
#include "adc.h"
#include "touch.h"
#include "joy.h"
#include "oled.h"
#include "sram.h"
#include "menu.h"
#include "spi.h"
#include "mcp.h"
#include "can.h"
#include "MCP2515.h"

int main(void)
{
	MCUCR |= (1<<SRE);	//Enable external memory
	SFIOR |= (1<<XMM2);
	
	uart_init(9600, NODE_1);
	adc_init();
	oled_init();
	oled_print_string("Please calibrate joystick!",0,0,8,0);
	oled_refresh();
	joy_init();
	touch_init();
	menu_init();
	spi_master_init();
	mcp_init();
	can_init(MODE_LOOPBACK); 
	oled_clear_screen();
	oled_refresh();
	
	//mcp_test();
	//sram_test(); Not ok...

	while (1)
	{
		Position position = joy_get_position();
		int position_length = sizeof(position);
		can_transmit(42, (char *)&position, position_length);
		
		//for loopback mode:
		Position position_received = *(Position*)can_receive();
		printf("x:%4d\ty:%4d\tz:%4d\r",position_received.x,position_received.y,position_received.z);
	
		menu_run_display();
		_delay_ms(100);	
	}
}	
	
	/*--NOTES--
	1 output, 0 input

	JTAG interface with the 168
	Left to right (row 41-44)
	1,5,3,9
	Vcc 4
	GND 2

	char test[] = "raoul&he"; DO NOT USE THIS
	char* test = "raoul&he"; USE THIS. Adds \0 termination automatically

	printf("%30s",string); Variable string will take fixed amount of space by padding with spaces
	printf("%4d",integer); Variable digit will take fixed amount of space by padding with spaces

	printf("\b"); Backspace
	printf("\n"); Line Feed
	printf("\r"); Carriage Return
	Only works properly in putty
	\r\n is a proper new line! This combination works in Putty, Bray's and Termite
	*/

	
	
	
	/*--TODO--
	-Add a brightness function
	-Add a interrupt for 60 Hz updates

	-can_transmitt(msg);
		-can_msg_t msg = can_receive();
		-position3.x = msg.data[0];

	Tips from student assistant:
	-sleep(); avr/sleep.h? Don't run main loop more often than necessary
		-Read about various sleep states. Sleep state for ADC reading? Reduces noise
	-EEPROM Usage?
	-Use fprintf and set up multiple streams
		-fprintf(&uart_print,"",string);
		
	Suggestion for various testing functions:
	
	int module_test(){
		printf("Testing module:");
		int input = ;
		int output = ;
		int expected = ;
		do_something(input);
		output = check_results();
		if (output == expected){
			printf("got %s, expected %s ...OK", output, expected);
			return 0;
		}
		else{
			printf("got %s, expected %s ...NOT OK", output, expected);
			return 1;
		}
	}
	
	Desired result:
	Testing MCP		... OK
	Testing SPI		... OK
	Testing RAM		... OK
	Testing ADC		... OK
	Testing Oled	... (printed "OK")
	Testing CAN_LOOPBACK ...OK
	*/
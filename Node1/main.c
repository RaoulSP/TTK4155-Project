#include <util/delay.h>
#include <avr/io.h>
#include "../lib/uart.h"
#include "../lib/can.h"
#include "../lib/adc.h"
#include "../lib/joy.h"
#include "oled.h"
#include "sram.h"
#include "menu.h"
#include "touch.h"
#include "interrupt_flags.h"

//IDs:
//1 = Position 
//2 = Score
//3 = Status
//4 = PID calibration?
//5++ = Random strings


//Interrupt flags
volatile int can_message_received = 1;
volatile int game_score_received = 0;
volatile int game_second_passed = 0;
volatile int oled_refresh_timer = 0;
State state = in_menu;



int main(void)
{
	MCUCR |= (1<<SRE);	//Enable external memory
	SFIOR |= (1<<XMM2); 
	
	uart_init(9600);
	can_init(MODE_NORMAL); 
	adc_init();
	oled_init();
	joy_init();
	touch_init();
	menu_init();
	sei();
	
	
	
	while (1)
	{	
		switch (state)
		{
			case in_menu:
				if(oled_refresh_timer == 1){
					menu_run_display();
					oled_refresh_timer = 0;
				}
				
				break;
			case in_game:		
				game_run();
				break;
		}
		
	}
}	
	
	
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
		module_write(input); //or do something similar
		output = module_read();
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
	Testing Oled	... (Print "OK" on OLED)
	Testing CAN_LOOPBACK ...OK
	etc.
	
	
	#include "common.h"?
	
	Boot nr.: in eeprom?
	
	coordinate struct? containing x  and y (and z? ... mwahaha)
	*/
	
	
	
	
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
	Only works properly in PuTTY
	\r\n is a proper new line! This combination works in Putty, Bray's and Termite
	
	sizeof(int) = 2 bytes
	*/
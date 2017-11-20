#include <util/delay.h>
#include <avr/io.h>

#include "../lib/uart.h"
#include "../lib/can.h"
#include "../lib/adc.h"
#include "../lib/joy.h"
#include "../lib/interrupt_flags.h"
#include "oled.h"
#include "sram.h"
#include "menu.h"
#include "touch.h"


//IDs:
/*
typedef enum can_id_t {
	COMMAND, //can ask for status, "on/off and calibrated or not"
	STATUS,
	STRING,
	OCCLUDED,
	POSITION
} Can_id;
*/
//1 = Command 
//2 = Status
//3 = String
//4 = Occluded
//5 = Position
//4 = PID calibration?
//5++ = Random strings

//Interrupt flags

volatile int game_occluded = 0;
volatile int game_second_passed = 0;
volatile int oled_refresh_timer = 0;
volatile int game_interrupt_flag = 1;
State state = in_game;

int main(void)
{
	MCUCR |= (1<<SRE);	//Enable external memory
	SFIOR |= (1<<XMM2); 
	
	uart_init(9600);
	printf("\033[4m\r\nreset\033[0m\r\n");
	can_init(MODE_NORMAL); 
	adc_init();
	oled_init();
	joy_init();
	touch_init();
	menu_init();
	sei();
	
	while (1){	
		switch (state){
			case in_menu:
				if(oled_refresh_timer == 1){
					menu_run_display();
					oled_refresh_timer = 0;
				}
				break;
			case in_game:		
				if(game_interrupt_flag == 1){
					game_run();
				}
				break;
		}
	}
}	
	
	
	/*--TODO--
	-Add a brightness function
	-Add a interrupt for 60 Hz updates CHECK

	-can_transmit(msg); CHECK
		-can_msg_t msg = can_receive();
		-position3.x = msg.data[0];

	Tips from student assistant:
	-sleep(); avr/sleep.h? Don't run main loop more often than necessary
		-Read about various sleep states. Sleep state for ADC reading? Reduces noise
	-EEPROM Usage?
	-Use fprintf and set up multiple streams
		-fprintf(&uart_print,"",string);
	
	-Add capacitor on Node2 extension - solenoid affects stuff
	-IMPORTANT: SOLENOID TIMER DOES NOT WORK ATM
	
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
	
	
	can_receive in main, use game_occluded
	implement bitfield for struct
	ctrl + f for "delay" and replace most of them
	*/
	
	
	
	
	/*--NOTES--
	1 output, 0 input

	JTAG interface with the 162
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
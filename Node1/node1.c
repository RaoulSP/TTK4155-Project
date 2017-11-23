#include "../lib/settings.h"
#include <avr/io.h>
#include <stdlib.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "../lib/interrupts.h"
#include "../lib/uart.h"
#include "../lib/can.h"
#include "../lib/adc.h"
#include "../lib/joy.h"
#include "oled.h"
#include "sram.h"
#include "menu.h"
#include "touch.h"
#include "game.h"
#include "music.h"

//Global variables
volatile Flags flags = {0,0,0,0,0,0};
State state = in_menu;

int main(void)
{
	MCUCR |= (1<<SRE);	//Enable external memory
	SFIOR |= (1<<XMM2); 
	uart_init(9600);
	printf("\r\nRESET\r\n");
	can_init(MODE_NORMAL);
	adc_init();
	oled_init();
	joy_init();
	touch_init();
	music_init();
	menu_init();
	sei();
	
	Position position_received = {0,0,0,0};
	Msg msg_received;
	
	while (1)
	{	
		if(flags.music_beat == 1){
			music_play_song();
			flags.music_beat == 0;
		}
		
		switch (state){
			case in_menu:
				if(flags.oled_refresh_timeout == 1){
					menu_run_display();
					flags.oled_refresh_timeout = 0;
				}
				break;
			case in_game:		
				if(flags.game_interrupt == 1){
					game_run();
					flags.game_interrupt == 0; //set to 0 when we start using interrupts
				}
				break;
		}
		
		if (can_message_received == 1){
			msg_received = can_receive();
			switch (msg_received.id){
				case OCCLUDED:
					flags.game_occluded = 1;
					break;
				case POSITION:
					position_received = *(Position*)msg_received.data;
					printf("%4d\r", position_received.y);
					break;
				default:
					printf("ID unknown\r");
			}
			free(msg_received.data);
			can_message_received = 0;
		}
		
	sleep_enable(); //reduces power draw by 5-6 mA?
	sleep_cpu();
	sleep_disable(); //SMCR &= ~(1<<SE);
	}
}	

	
	
	/*--TODO--
	-Add a brightness function
X	-Add a interrupt for 60 Hz updates

X	-can_transmit(msg); CHECK
X		-can_msg_t msg = can_receive();
X		-position3.x = msg.data[0];

	Tips from student assistant:
X	-sleep(); avr/sleep.h? Don't run main loop more often than necessary
		-Read about various sleep states. Sleep state for ADC reading? Reduces noise
	-EEPROM Usage?
	-Use fprintf and set up multiple streams
		-fprintf(&uart_print,"",string);
	
X	-Add capacitor on Node2 extension - solenoid affects stuff
X	-IMPORTANT: SOLENOID TIMER DOES NOT WORK ATM
	
	-#include "common.h"?
	
	-Boot nr.: in eeprom?
	
	-coordinate struct? containing x  and y (and z? ...)
	
X	-can_receive in main, use flags.game_occluded
	-implement bitfield for struct
X	-ctrl + f for "delay" and replace most of them
	
	//Nov. 22nd
	
	-music --> buzzer?

	-state initialize?

	-can_receive and switch?
	-can_init--> associate ID with storage address

	-coord struct med x og y for pixler. bitfield?
	-bitfield for alle flagg

	-strings.h

X	-preprosessor else if node 2?

	-node2_init?

X	-remove us delays`

	-adc og ext_adc?

X	-adc switch on channel?

	-uint8_t or int8_t
		-etc

	-common for interrupts and other things?

	-ctrl + F "int"
	
	-Mark things as const. Const parameters in functions?
	
	-Mark things as unsigned
	
X	-Run_game on timer instead of interrupts?
	*/
	
	/*
	Suggestion for various testing functions:
	
	/	int module_test(){
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
	
	/*
	State state;
	
	while (1){
		switch (state){
			case in_menu:
			if(flags.oled_refresh_timeout == 1){
				menu_run_display();
				flags.oled_refresh_timeout = 0;
			}
			break;
			case in_game:
			if(flags.game_interrupt == 1){
				game_run();
			}
			break;
			
			case menu_main:
			oled_print_string("Main menu")
			menu_add_link(&options,page);
			menu_add_link(&about,popup);
			if(z_pressed){
				state = selected;
			}
			
			break;
			
			case menu_main:
			draw_page(main_menu)?
			break;
		}
		if(z_pressed){
			state = selected.state?;
		}
		if(down_pressed){
			state = selected.state?;
		}
	}
	*/
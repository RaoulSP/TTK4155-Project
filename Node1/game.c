#include "game.h"
#include "../lib/joy.h"
#include "../lib/uart.h"
#include "../lib/can.h"
#include "interrupt_flags.h"
#include "../lib/settings.h"


int score = 0;
int initiated = 0;
int game_time = 0;

void game_init(){
	/*
	int calibrated = 0;
	while (!calibrated){
		Msg msg_received = can_receive();
		if(msg_received.id == 3){
			int calibrated = 1;
		}
		free(msg_received.data);	
	}
	*/
	
	TCCR3A = (1 << WGM31) | (1 << WGM30);				//Compare match mode
	TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31); //clock source to be used by the Timer/Counter clkI/O/8
	ETIMSK = (1 << OCIE3A);								//Interrupt on compare match
	int OCRA_num = (long)F_CPU/(8); //Prescale with 256 to get seconds
	OCR3AH = OCRA_num >> 8;
	OCR3AL = OCRA_num; //Sets the value for the compare match to 10240
}


void game_run(){
	if (!initiated){
		game_init();
		initiated = 1;
	}
	   
	if(game_second_passed == 1){
		printf("%d\r\n", game_second_passed);
		game_time++;
		char str[5];
		sprintf(str,"%5d",game_time);
		oled_clear_screen();
		oled_print_string(str,0,0,8,0);
		oled_print_string("Herman er best",0,1,8,0);
		oled_refresh();
		game_second_passed = 0;
		
	}
	//SEND POSITION
	Position position = joy_get_position();
	Msg msg;
	msg.id = 42;
	msg.length = sizeof(position);
	msg.data = (char*) &position;
	can_transmit(msg);
	
	//GET SCORE
	if (can_message_received){
		Msg msg_received = can_receive();
		if(msg_received.id == 2){
			score++;
		}
		free(msg_received.data);
	}
}

ISR(TIMER3_COMPA_vect)
{
	game_second_passed = 1;
}
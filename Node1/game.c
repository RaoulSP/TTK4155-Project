#include "oled.h"
#include "game.h"
#include "../lib/joy.h"
#include "../lib/uart.h"
#include "../lib/can.h"
#include "../lib/interrupt_flags.h"
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
	
	int OCRA_num = (long)F_CPU/(8);						//Prescale with 256 to get seconds
	OCR3AH = OCRA_num >> 8;
	OCR3AL = OCRA_num;									//Sets the value for the compare match to 10240
	TCCR3A = (1 << WGM31) | (1 << WGM30);				//Compare match mode
	TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31); //clock source to be used by the Timer/Counter clkI/O/8
	ETIMSK = (1 << OCIE3A);								//Interrupt on compare match
}


void game_run(){
	if (!initiated){
		game_init();
		initiated = 1;
	}
	   
	if(game_second_passed == 1){
		game_time++;
		char str[5];
		sprintf(str,"%5d",game_time);
		//oled_clear_line(1);
		oled_print_string(str,0,0,8,0);
		oled_print_string("raoHan er best",0,1,8,0);
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
	//can_transmit(can_construct_msg(42, sizeof(position), (char*) &position));
	
	//GET SCORE
	if (can_message_received){
		Msg msg_received = can_receive();
		if(msg_received.id == OCCLUDED){
			score++;
			char scr[5];
			sprintf(scr,"%5d",score);
			oled_print_string(scr,5,5,8,0);
			oled_refresh();
			printf("%d\r",score);
		}
		free(msg_received.data);
	}
	can_message_received = 0;
}

ISR(TIMER3_COMPA_vect){
	game_second_passed = 1;
}
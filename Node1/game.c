#include "oled.h"
#include "game.h"
#include <stdio.h>
#include "../lib/joy.h"
#include "../lib/uart.h"
#include "../lib/can.h"
#include "../lib/interrupts.h"
#include "../lib/settings.h"
#include "music.h"
int deaths = 0;
int initiated = 0;
int game_time = -1;

void game_init(){
	oled_clear_screen();
}
int tones[] = { 1898, 1690, 1500, 1420, 1265, 1194, 1126, 1063, 1001, 947, 893, 843, 795, 749, 710, 668, 630, 594 };

void game_run(){
	
	if (!initiated){
		game_init();
		//music_init(1);
		//music_start_song();
		initiated = 1;
	}
	
	
	if(music_beat_flag == 1){
		music_play_song(tones, 15);
		music_beat_flag == 0;
	}
		//music_play_song(tones,15);
	if(game_time_passed == 1){
		game_time++;
		char str[5];
		char hs[4];

		sprintf(str,"%5d",game_time);
		sprintf(hs,"%4d",1337);
		oled_clear_line(0);
		oled_print_string("HIGH SCORE:",0,0,5,0);
		oled_print_string(hs,7*8,0,5,0);
		oled_clear_line(1);
		oled_print_string("SCORE:",0,1,5,0);
		oled_print_string(str,4*8,1,5,0);
		oled_refresh();
		game_time_passed = 0;
		
	}
	//SEND POSITION
	Position position = joy_get_position();
	Msg msg;
	msg.id = 42;
	msg.length = sizeof(position);
	msg.data = (char*) &position;
	can_transmit(msg);
	
	//GET SCORE
	if (game_occluded){
		char scr[5];
		deaths++;
		sprintf(scr,"%5d",deaths);
		oled_print_string(scr,5,5,8,0);
		oled_refresh();
		game_occluded = 0;
		//TODO: stop game
	}
	//TODO: Create state-machine
	//TODO: Calibrate hardware (?)
}


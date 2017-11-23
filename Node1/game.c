#include <stdio.h>

#include "oled.h"
#include "game.h"
#include "music.h"
#include "../lib/joy.h"
#include "../lib/uart.h"
#include "../lib/can.h"
#include "../lib/interrupts.h"
#include "../lib/settings.h"

int score = 0;
int deaths = 0;
int initiated = 0;
int game_time = -1;
int high_score = 0;

char* player_name = "Hal9000";
Game_state game_state = INITIALIZING;

//Menus for the game
Menu game_over = {
	.name = "GAME OVER",
	.draw = draw_menu,
	.offset = 2
};

Menu choose_player = {
	.name = "CHOOSE PLAYER",
	.draw = draw_menu,
	.offset = 1
};

//Actions for the menu options in the game
Menu* end_game(Menu* self){
	state = in_menu;
	return self;
}

Menu* replay_game(Menu* self){
	game_state = INITIALIZING;
	return self;
}

Menu* start_game(Menu* self){
	player_name = self->name;
	game_state = PLAYING;
	return self;
}

void game_init(){
	game_state = PLAYER_SELECTION;
	game_time = -1;
	//Choose player menu
	char* player_list[4] = {"Peter","Herman","Raoul","Hans"};
	
	add_list_entries(&choose_player,player_list,4);
	choose_player.sub_menus[0]->action = start_game;
	choose_player.sub_menus[1]->action = start_game;
	choose_player.sub_menus[2]->action = start_game;
	choose_player.sub_menus[3]->action = start_game;
	
	//Game over menu
	char* end_of_game_options[2] = {"Continue","End game"};
	add_list_entries(&game_over,end_of_game_options,2);
	game_over.sub_menus[0]->action = replay_game;
	game_over.sub_menus[1]->action = end_game;
}
int tones[] = { 1898, 1690, 1500, 1420, 1265, 1194, 1126, 1063, 1001, 947, 893, 843, 795, 749, 710, 668, 630, 594 };

void game_run(){
	printf("%d\n",game_state);
	switch(game_state){
		case INITIALIZING:
			game_init();
			//music_init(); Maybe?
			break;
		
		case PLAYER_SELECTION:
			if(flags.game_time_passed == 1){
				oled_clear_screen();
				choose_player.draw(&choose_player);
				oled_refresh();
				flags.game_time_passed = 0;
			}
			break;
		
		case PLAYING:
			if(flags.game_time_passed == 1){
				//Update music?
				Position position = joy_get_position();
				Msg msg;
				msg.id = POSITION;
				msg.length = sizeof(position);
				msg.data = (char*) &position;
				can_transmit(msg);
				
				//UPDATING THE SCREEN
				char h_score_temp[5];
				char score_temp[5];
				game_time++;
				//sprintf(stemp,"%5d",game_time);
			
				oled_clear_screen();
				oled_print_string("HIGH SCORE:",0,0,5,0);
				oled_print_string("SCORE:",0,1,5,0);
				sprintf(h_score_temp,"%5d", high_score);
				oled_print_string(h_score_temp,128-5*8,0,5,0);
				sprintf(score_temp,"%5d", game_time);
				oled_print_string(score_temp,128-5*8,1,5,0);
				oled_refresh();
				flags.game_time_passed = 0;
			}
		
			if(flags.game_occluded){
				//Checking if the ball is blocking the sensor
				score = game_time;
				game_state = GAME_OVER;
			}
			break;
		
		case GAME_OVER:
			if(flags.game_time_passed == 1){
				oled_clear_screen();
				char score_temp[5];
				sprintf(score_temp,"%5d",score);
				if(score > high_score){
					oled_print_string("NEW HIGH SCORE: ",0,1,5,0);
					//TODO: Store in EEPROM
				}
				else{
					oled_print_string("SCORE: ",0,1,5,0);
				}
				oled_print_string(score_temp,128-5*8,1,5,0);
				game_over.draw(&game_over); //With offset
				oled_refresh();
				flags.game_time_passed = 0;
			}
		
			break;
	}
	
	//TODO: Create state-machine
	//TODO: Calibrate hardware (?)
}
/*---Changes---
-Changes the run game to be a switch.
-Added game-states in header
-Added game menus and corresponding functions
-Added offset in menu struct
-Changed menu draw to accommodate the offset

TODO:
-Store score between runs
-Make menu scroll
-Make menu handle drawing entries outside of screen (not try to do it)
-Changed transition in menu to handle wrapping better
*/

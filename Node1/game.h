
#ifndef GAME_H_
#define GAME_H_
#include "menu.h"
void game_init();
void game_run();

typedef enum game_state_t{
	INITIALIZING,
	PLAYING,
	GAME_OVER,
	PLAYER_SELECTION
} Game_state;

//Game menu actions
Menu* end_game(Menu* self);
Menu* replay_game(Menu* self);
Menu* start_game(Menu* self);

#endif /* GAME_H_ */
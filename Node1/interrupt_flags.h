/*
 * interrupt_flags.h
 *
 * Created: 17.11.2017 22:00:29
 *  Author: hansae
 */ 


#ifndef INTERRUPT_FLAGS_H_
#define INTERRUPT_FLAGS_H_

typedef enum state_t{
	in_menu,
	in_game
} State;

#include <avr/interrupt.h>

 extern volatile int can_message_received;
 extern volatile int game_score_received;
 extern volatile int game_second_passed;
 extern volatile int oled_refresh_timer;
 
 extern State state;

#endif /* INTERRUPT_FLAGS_H_ */
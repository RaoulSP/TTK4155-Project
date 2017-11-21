#ifndef INTERRUPT_H_
#define INTERRUPT_H_
#include <avr/interrupt.h>
#include "settings.h"

#ifdef NODE_1
typedef enum state_t{
	in_menu,
	in_game
} State;

 extern volatile int game_occluded;
 extern volatile int game_time_passed;
 extern volatile int game_interrupt_flag;
 extern volatile int oled_refresh_timer;
 extern volatile int music_beat_flag;
 extern State state;
 
#else

extern volatile int solenoid_timer;
extern volatile int pid_timer;

#endif
extern volatile int can_message_received;

#endif /* INTERRUPT_H_ */
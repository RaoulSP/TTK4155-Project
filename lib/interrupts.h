#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include <avr/interrupt.h>

#include "settings.h"

#ifdef NODE_1
typedef enum state_t{
	in_menu,
	in_game
} State;

extern State state;

extern volatile int can_message_received;

typedef struct flag_t{
	int game_occluded : 1;
	int game_time_passed : 1;
	int game_interrupt : 1;
	int oled_refresh_timeout : 1;
	int music_beat : 1;
	int can_message_received : 1;
}Flags;
#endif

#ifdef NODE_2
typedef struct flag_t{
	int solenoid_timeout : 1;
	int pid_timeout : 1;
	int can_message_received : 1;
}Flags;
#endif

extern volatile Flags flags;
/*
	 extern volatile int flags.flags.flags.game_occluded : 1;
	 extern volatile int flags.flags.flags.game_time_passed : 1;
	 extern volatile int game_interrupt_flag : 1;
	 extern volatile int oled_refresh_timer : 1;
	 extern volatile int flags.music_beat : 1;
	 
*/
#endif /* INTERRUPT_H_ */
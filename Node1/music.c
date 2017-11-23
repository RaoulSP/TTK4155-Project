/*
 * music.c
 *
 * Created: 21.11.2017 13:39:57
 *  Author: hansae
 */ 

#include "../lib/pwm.h"
#include "../lib/interrupts.h"
#include <util/delay.h>
#include "music.h"


int song_count = 0;
int length = 0;

Note song[10] = {{.note = Db3, .duration = 1},
				 {.note = D0,  .duration = 1},
				 {.note = Eb0, .duration = 2},
				 {.note = E0,  .duration = 0.5},
				 {.note = F0,  .duration = 1},
				 {.note = Gb0, .duration = 4},
				 {.note = G0,  .duration = 1},
				 {.note = C5,  .duration = 0.4},
				 {.note = E5,  .duration = 2},
				 {.note = F4,  .duration = 1}};
	
	
void music_init(){
	pwm_init();
	double T = 0;		//Time between notes
	int OCRA_num = (double)F_CPU*T/(2.0*256.0)-1.0;
	
	//Prescale with 256 to get seconds
	OCR3AH = OCRA_num >> 8;
	OCR3AL = OCRA_num;									//Sets the value for the compare match to 10240
	TCCR3A = (1 << WGM31) | (1 << WGM30);				//Compare match mode
	TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS12);	//clock source to be used by the Timer/Counter clkI/O/8
	ETIMSK = (1 << OCIE3A);								//Interrupt on compare match
	
}

void tone(double note, double duration){
	//TCCR3B &= ~(1 << CS12);	//Disable timer
	//pwm_change_freq(0);
	//int OCRA_num = (double)F_CPU*duration/(2.0*256.0)-1.0; //Max 6 seconds
	//OCR3AH = OCRA_num >> 8;	//New top to count to
	//OCR3AL = OCRA_num;
	//TCNT3H = 0;	//Counting register to 0
	//TCNT3L = 0;
	pwm_change_freq(note);
	//TCCR3B |= (1 << CS12); //Enable timer
}

void music_play_song(){
	if(song_count == length){
		song_count = 0;
	}
	tone(song[song_count].note,song[song_count].duration);
	song_count++;
}

//TODO: Change time for music interrupts 
//-Something weird happening

ISR(TIMER3_COMPA_vect){
	flags.music_beat = 1;
}

//In game!!
//if(flags.music_beat == 1){
//	music_play_song(song, 15);
//	flags.music_beat == 0;
//}
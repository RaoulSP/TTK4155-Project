/*
 * music.c
 *
 * Created: 21.11.2017 13:39:57
 *  Author: hansae
 */ 

#include "../lib/pwm.h"
#include "../lib/interrupts.h"
#include <util/delay.h>

volatile int music_beat_flag = 0;
int song_count = 0;

void music_init(int bpm){
	pwm_init();
	//Interrupt
	//Game second passed
	double f = 1.0/((double)bpm/60.0);
	int OCRA_num = ((uint32_t)F_CPU/(uint16_t)(f * 256) - 1);
	
	(long)F_CPU/(256);					//Prescale with 256 to get seconds
	OCR3AH = OCRA_num >> 8;
	OCR3AL = OCRA_num;									//Sets the value for the compare match to 10240
	TCCR3A = (1 << WGM31) | (1 << WGM30);				//Compare match mode
	TCCR3B = (1 << WGM33) | (1 << WGM32); //clock source to be used by the Timer/Counter clkI/O/8
	ETIMSK = (1 << OCIE3A);								//Interrupt on compare match
}

void music_start_song(int bpm){
	TCCR3B |= (1 << CS32);
}

//Mby have play song in interrupt for perfect timing?
void music_play_song(int song[], int length){
	if(song_count == length){
		song_count = 0;
		TCCR3B &= ~(1 << CS32);
		//Stop interrupt timer
	}
	pwm_change_freq(song[song_count]);
	song_count++;
}

//TODO: Change time for music interrupts 
//-Something weird happening

ISR(TIMER3_COMPA_vect){
	music_beat_flag = 1;
}
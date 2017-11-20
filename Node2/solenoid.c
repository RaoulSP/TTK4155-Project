
#include <avr/io.h>
#include <util/delay.h>
#include "../lib/interrupt_flags.h"

void solenoid_init(){
	DDRE |= (1 << PE4); 
	PORTE |= (1 << PE4);
	
	//THIS DOES NOT WORK
	TCCR4A |= (1 << WGM41) | (1 << WGM40);				//Compare match mode
	TCCR4B |= (1 << WGM43) | (1 << WGM42) | (1 << CS42); //clock source to be used by the Timer/Counter clkI/O/64
	OCR4AH = 0b01111010;
	OCR4AL = 0b10011000; //Sets the value for the compare match to 15000
}

//Turn solenoid on/off very quickly
void solenoid_kick(){
		PORTE &= ~(1 << PE4);
		TIMSK4 |= (1 << OCIE4A);
		solenoid_timer = 1;
	 //delay is needed. Min 10ms
//To do: add interrupt based timer
}

ISR(TIMER2_COMPA_vect){
	PORTE |= (1 << PE4);
	TIMSK4 &= ~(1 << OCIE4A);
	solenoid_timer = 0;
}
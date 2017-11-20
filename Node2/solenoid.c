
#include <avr/io.h>
#include <util/delay.h>
#include "../lib/interrupt_flags.h"

void solenoid_init(){
	DDRE |= (1 << PE4); 
	PORTE |= (1 << PE4);
	
	TCCR4A |= (1 << WGM41) | (1 << WGM40);				//Compare match mode
	TCCR4B |= (1 << WGM43) | (1 << WGM42); //clock source to be used by the Timer/Counter clkI/O/64
	OCR4AH = 0b00111010;
	OCR4AL = 0b10011000; //Sets the value for the compare match to 15000
	
	//Enable interrupt
	TIMSK4 |= (1 << OCIE4A);
}


void solenoid_kick(){
	//Turn solenoid on
	PORTE &= ~(1 << PE4);
	
	//Start timer
	TCCR4B |= (1 <<  CS42);
	solenoid_timer = 1;
}

ISR(TIMER4_COMPA_vect){
	solenoid_timer = 0;
	//Turn off solenoid
	PORTE |= (1 << PE4);
	//Stop timer
	TCCR4B &= ~(1 << CS42);

}
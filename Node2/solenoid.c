#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>


void solenoid_init(){
	DDRE |= (1 << PE4); 
	PORTE |= (1 << PE4);
}


//Turn solenoid on/off very quickly
void solenoid_kick(){
	PORTE &= ~(1 << PE4);
	_delay_ms(500); //delay is needed. Min 10ms
	PORTE |= (1 << PE4);
}
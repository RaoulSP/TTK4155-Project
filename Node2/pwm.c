
#include <avr/io.h>
#include "pwm.h"
#define F_CPU 16000000
uint16_t pwm_top = 0;

void pwm_init(){
	TCCR1A |= (1 << WGM11) | (1 << COM2A1); //Sets mode to fastpwm and prescaler clock 8 //Initial values all 0
	TCCR1B |= (1 << WGM13) | (1 << WGM12) |  (1 << CS11);
	DDRB |= (1 << PB5); //Set direction for PWM pin to output
	
	uint16_t f = 50; //f = 1/20ms
	uint16_t prescaler = 8;
	pwm_top = ((uint32_t)F_CPU/(uint16_t)(f * prescaler) - 1);
	ICR1 = pwm_top; //This defines the pwm period length
	pwm_set_duty_cycle(1.5);
	
}

void pwm_set_duty_cycle(float ms){
	if(ms > 20 || ms < 0){
		return -1;
	}
	
	//---Limits for the servo - TODO (?): move to motor driver module---
	if(ms > 2.1){
		ms = 2.1;
	}
	else if (ms < 0.9){
		ms = 0.9;
	}
	//------------------------------------------------------------------
	float duty_cycle = ms/20.0;
	OCR1A = (pwm_top*duty_cycle);
}
#include "../lib/settings.h"
#include "pwm.h"

uint16_t pwm_top = 0;

void pwm_init(){
	DDRB |= (1 << DDB5); //Set direction for PWM pin to output //PB5 = pin 11 on the Arduino shield
	TCCR1A |= (1 << WGM11) | (1 << COM2A1); //Sets mode to fast pwm and prescaler clock 8 //Initial values all 0
	TCCR1B |= (1 << WGM13) | (1 << WGM12) |  (1 << CS11);
	
	uint16_t f = 50; //Period 20ms --> f = 1/20ms
	uint16_t prescaler = 8;
	pwm_top = ((uint32_t)F_CPU/(uint16_t)(f * prescaler) - 1);
	ICR1 = pwm_top; //This defines the pwm period length
	pwm_set_duty_cycle(1.5); //Center the servo
}

void pwm_set_duty_cycle(float ms){
	if(ms <= 20 && ms >= 0){
		float duty_cycle = ms/20.0;
		OCR1A = (pwm_top*duty_cycle);
	}
}
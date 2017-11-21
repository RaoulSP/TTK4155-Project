#include "../lib/pwm.h"
#include "servo.h"

void servo_set_angle(float angle){
	int ms = angle; 
	
	if(ms > 2.1){
		ms = 2.1;
	}
	else if (ms < 0.9){
		ms = 0.9;
	}
	
	pwm_set_duty_cycle(ms);
	
	//TODO: map angle to ms

}


#include <util/delay.h>
#include <avr/interrupt.h>

#include "../lib/uart.h"
#include "../lib/can.h"
#include "../lib/joy.h"
#include "../lib/adc.h"
#include "../lib/pwm.h"
#include "servo.h"
#include "motor.h"
#include "TWI_Master.h"
#include "solenoid.h"
#include "PID.h"

volatile int pid_timer = 0;
volatile int solenoid_timer = 0;
int scoring_allowed = 1;

int main(void)
{	
	uart_init(9600);
	adc_init();
	can_init(MODE_NORMAL);
	pwm_init();
	TWI_Master_Initialise();
	motor_init();
	solenoid_init();
	
	sei();
	
	Position position_received = {0,0,0};
	int z_released = 1;
	Msg msg_received;
		
	while(1)
	{
		check_if_scored();
		
		//CAN RECEIVE
		msg_received = can_receive();
		switch (msg_received.id){
			case 1: //For short strings
				printf("%s",msg_received.data);
				break;
			case 13: //For sending integers
				printf("%d", *(int*)msg_received.data);
				break;
			case 42: //For sending positions
				position_received = *(Position*)msg_received.data;
				break;
			default:
				printf("ID unknown\r");
		}
		free(msg_received.data);
		
		//UPDATE MOTORS		
		int r_slider_mean = 128;
		int r_around_zero = position_received.r_slider - r_slider_mean;
		float r = r_around_zero*100/128;
		servo_set_angle((((float)r) * 1.2 / 200.0) + 1.5);
		
		printf("r = %d\r\n", position_received.r_slider);
		
		if (position_received.z == 1 && z_released == 1 && solenoid_timer == 0){
			solenoid_kick();
			z_released = 0;
		}
		if(position_received.z == 0){
			z_released = 1;
		}
		
		if (pid_timer == 1) {
			motor_move_with_pid(position_received.y);
			pid_timer = 0;
		}
	}
}

int check_if_scored()
{
	int lower_trigger_value = 300;
	int upper_trigger_value = 600;
	int adc_val = adc_read();
	
	if (adc_val < lower_trigger_value && scoring_allowed){ //Can't score again until the adc value goes up
		can_transmit(can_construct_msg(OCCLUDED, 0, 0));
		scoring_allowed = 0;
		return 1;
	}
	else if (adc_val >= upper_trigger_value){
		scoring_allowed = 1;
	}
	return 0;
}

/* ----TODO----

-Make node 2 sleep until the game starts and make it sleep when it ends. Connect interrupt from
 I/O board to ext int on arduino

-Check if scored should be called something else


*/
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../lib/uart.h"
#include "../lib/can.h"
#include "../lib/joy.h"
#include "../lib/adc.h"
#include "pwm.h"
#include "motor_driver.h"
#include "TWI_Master.h"
#include "solenoid.h"
#include "PID.h"

volatile int pid_timer = 0;
volatile int solenoid_timer = 0;
int scoring_allowed = 1;


int main(void)
{	
	uart_init(9600);
	printf("\033[4m\r\nreset\033[0m\r\n");
	adc_init();
	can_init(MODE_NORMAL);
	pwm_init();
	TWI_Master_Initialise();
	motor_init();
	solenoid_init();
	motor_move_dc(0);
	sei(); //enable the use of interrupts
	
	motor_move_dc_with_pid(0);
	
	Position position_received = {0,0,0};
	Msg msg_received;
	int score = 0;
	int z_released = 1;
	
	while(1)
	{
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
		score += check_if_scored();
		
		motor_move_servo(((-(float)position_received.y) * 1.2 / 200.0) + 1.5); //Maps -100,100 to 0.9,2.1

		if (position_received.z == 1 && z_released == 1 && solenoid_timer == 0){
			solenoid_kick();
			z_released = 0;
		}
		else if(position_received.z == 0){
			z_released = 1;
		}
		
		if (pid_timer == 1) {
			motor_move_dc_with_pid(position_received.y);
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
		scoring_allowed = 0;
		can_transmit(can_construct_msg(OCCLUDED, 0, 0));
		printf("OCCLUDED\r");
		return 1;
	}
	else if (adc_val >= upper_trigger_value){
		scoring_allowed = 1;
		printf("........\r");
	}
	return 0;
}
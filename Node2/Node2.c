#include "../lib/settings.h"
#include <avr/io.h>
#include <stdlib.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../lib/interrupts.h"
#include "../lib/uart.h"
#include "../lib/can.h"
#include "../lib/adc.h"
#include "../lib/joy.h"
#include "../lib/pwm.h"
#include "TWI_Master.h"
#include "solenoid.h"
#include "servo.h"
#include "motor.h"
#include "PID.h"

volatile Flags flags = {0,0,0};
int scoring_allowed = 1;

int main(void)
{	
	uart_init(9600);
	printf("\r\nRESET\r\n");
	TWI_Master_Initialise();
	motor_init();
	pwm_init();
	solenoid_init();
	can_init(MODE_NORMAL);
	adc_init();
	sei();
	
	Position position_received = {0,0,0,0};
	int z_released = 1;
	Msg msg_received;
		
	while(1)
	{
		//check_occlusion();
		
		if (/*can_message_received == 1*/1){
			msg_received = can_receive();
			switch (msg_received.id){
				case OCCLUDED:
					printf("received.\r\n");
					break;
				case POSITION:
					position_received = *(Position*)msg_received.data;
					printf("%4d\r", position_received.y);
					break;
				default:
					printf("ID unknown\r");
			}
			free(msg_received.data);
			can_message_received = 0;	
		}
		
		//Loopback:
		//can_transmit(can_construct_msg(OCCLUDED, 0, 0));
		//_delay_ms(10);
		servo_set_angle((((float)position_received.r_slider) * 1.2 / 100.0) + 0.9);
		//servo_set_angle((((float)position_received.y) * 1.2 / 200.0) + 1.5);
		
		if (position_received.z == 1 && z_released == 1 && flags.solenoid_timeout == 0){
			solenoid_kick();
			z_released = 0;
		}
		if(position_received.z == 0){
			z_released = 1;
		}
		if (flags.pid_timeout == 1) {
			motor_move_with_pid(position_received.y);
			flags.pid_timeout = 0;
		}
		
		sleep_enable(); //reduces power draw by 5-6 mA?
		sleep_cpu();
		sleep_disable(); //SMCR &= ~(1<<SE);
		
	}
	
}

void check_occlusion(){
	int lower_limit = 300;
	int upper_limit = 600;
	int adc_val = adc_read();
	
	if (adc_val < lower_limit && scoring_allowed){
		can_transmit(can_construct_msg(OCCLUDED, 0, 0));
		scoring_allowed = 0;
	}
	else if (adc_val >= upper_limit){
		scoring_allowed = 1;
	}
}


/* ----TODO----

-Make node 2 sleep until the game starts and make it sleep when it ends. Connect interrupt from
 I/O board to ext int on arduino

-Check if scored should be called something else


*/
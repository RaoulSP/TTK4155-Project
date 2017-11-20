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
	sei(); //enable the use of interrupts
	uart_init(9600);
	adc_init();
	can_init(MODE_NORMAL);
	pwm_init();
	TWI_Master_Initialise();
	motor_init();
	solenoid_init();

	//DAC module not used!
	
	
	Position position_received = {0,0,0};
	int z_released = 1;
	Msg msg_received;
	int i = 0;
	while(1)
	{
		i++;
		//printf("%d\r\n",i);
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
		
		//printf("%d\r\n",position_received.r_slider);
		//UPDATE MOTORS		
		int r_slider_mean = position_received.r_slider/2;
		int r_around_zero = position_received.r_slider - r_slider_mean;
		float r = r_around_zero*100/128;
		motor_move_servo((((float)r) * 1.2 / 200.0) + 1.5);
		printf("%d\r\n",r);
		
		
		//motor_move_servo((((float)position_received.y) * 1.2 / 200.0) + 1.5); //Maps -100,100 to 0.9,2.1
		
		//printf("Z: %d Released: %d Timer: %d\r", position_received.z, z_released, solenoid_timer);
		//printf("%d\r\n", solenoid_timer);
		if (position_received.z == 1 && z_released == 1 && solenoid_timer == 0){ //Solenoid out?
			solenoid_kick();
			//printf("hei");
			z_released = 0;
		}
		if(position_received.z == 0){
			z_released = 1;
		}
		
		if (pid_timer == 1) {
			//motor_move_dc_with_pid(position_received.y);
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
		//can_transmit(can_construct_msg(OCCLUDED, 0, 0));
		//printf("OCCLUDED\r");
		return 1;
	}
	else if (adc_val >= upper_trigger_value){
		scoring_allowed = 1;
		//printf("........\r");
	}
	return 0;
}
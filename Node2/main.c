#include "../lib/uart.h"
#include "../lib/can.h"
#include "../lib/joy.h"
#include "../lib/adc.h"
#include "pwm.h"
#include "motor_driver.h"
#include "TWI_Master.h"
#include "solenoid.h"
#include "PID.h"

#include <util/delay.h>
#include <avr/interrupt.h>

struct GLOBAL_FLAGS {
	uint8_t pid_timer : 1;
} volatile global_flags = {0};

int scoring_allowed = 1;

ISR(TIMER3_COMPA_vect) //PID timer - called with interval 0.01s
{
	global_flags.pid_timer = 1;
}

int main(void)
{	
	// Set up timer, enable timer/counter compare match interrupt
	TCCR3A = (1 << WGM31) | (1 << WGM30);				//Compare match mode
	TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31); //clock source to be used by the Timer/Counter clkI/O/8
	TIMSK3 = (1 << OCIE3A);								//Interrupt on compare match

	OCR3AH = 0b01001110;
	OCR3AL = 0b00100000; //Sets the value for the compare match to 20000
	
	uart_init(9600);
	adc_init();
	can_init(MODE_NORMAL);
	pwm_init();
	TWI_Master_Initialise();
	motor_init();
	solenoid_init();
	sei(); //enable the use of interrupts
		
	//initialize PID
	double K_p = 0.075;
	double K_d = 0;
	double K_i = 0;
	pidData_t pid;
	
	pid_Init(K_p, K_i, K_d, &pid);
	int16_t discrete_voltage = 0;
	
	Position position_received;
	Msg msg_received;
	int score = 0;
	int z_released = 1;
	
	int motor_position = 0;
	int motor_position_past = 0;
	int motor_speed = 0;
	
	while(1)
	{
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
		
		score += check_if_scored();
		
		motor_move_servo((((float)position_received.y) * 1.2 / 200.0) + 1.5); //Maps -100,100 to 0.9,2.1

		if (position_received.z == 1 && z_released == 1)
		{
			solenoid_kick();
			z_released = 0;
		}
		else if(position_received.z == 0){
			z_released = 1;
		}
		
	
		if (global_flags.pid_timer == 1) {
			motor_position_past = motor_position;
			motor_position = motor_encoder_read();
			printf("%d\r\n",motor_position);
			motor_speed = motor_position - motor_position_past;
			
			printf("MOTOR SPEED= %d\r\n",motor_speed);
			printf("Pos_rec_x= %d\r\n\r\n",position_received.x*20);
			
			discrete_voltage = pid_Controller(position_received.y*20, motor_speed, &pid);

			if (discrete_voltage > 255)
			{
				discrete_voltage = 255;
				printf("Discrete voltage: %d\r\n", discrete_voltage);
			}
			else if (discrete_voltage < -255)
			{
				discrete_voltage = -255;
				printf("Discrete voltage: %d\r\n", discrete_voltage);
			}
			motor_move_dc(discrete_voltage);
			global_flags.pid_timer = 0;
		}
	}
	
	
}

int check_if_scored()
{
	int trigger_value = 200;
	int adc_val = adc_read();
	//printf("ADC VAL = %d\r\n", adc_val);
	if (adc_val < trigger_value && scoring_allowed){ //Can't score again until the adc value goes up
		scoring_allowed = 0;
		return 1;
	}
	else if (adc_val >= trigger_value){
		scoring_allowed = 1;
	}
	return 0;
}
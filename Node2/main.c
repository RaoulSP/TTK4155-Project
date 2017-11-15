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


//Interrupt flags
struct GLOBAL_FLAGS {
	uint8_t pid_timer : 1;
	uint8_t dummy : 7;
} volatile gFlags = {0, 0};


int scoring_allowed = 1;


//Interrupt routines
//PID timer - called with interval 0.01s
#define TIME_INTERVAL (long long)(0.01 * F_CPU) / 255
ISR(TIMER0_OVF_vect)
{
	static uint16_t i = 0;
	if (i < TIME_INTERVAL) {
		i++;
	} else {
		gFlags.pid_timer = 1;
		i                = 0;
	}
}

// Set up timer, enable timer/counter 0 overflow interrupt


int main(void)
{	
	// Set up timer, enable timer/counter 0 overflow interrupt
	TCCR0B = (1 << CS00); // clock source to be used by the Timer/Counter clkI/O
	TIMSK0 = (1 << TOIE0);
	TCNT0  = 0;
	
	uart_init(9600);
	adc_init();
	can_init(MODE_NORMAL);
	pwm_init();
	TWI_Master_Initialise();
	motor_init();
	solenoid_init();
	sei(); //enable the use of interrupts
		
	int score = 0;
	printf("PUTTY IS STILL ALIVE, YEAH!!!!!\r\n");
	
	
	
	//initialize PID
	//Gains
	double K_p = 0.09;
	double K_d = 0.00; // K_d is actually K_d/T
	double K_i = 0.000;
	pidData_t pid;
	
	pid_Init(K_p, K_i, K_d, &pid);
	int16_t discrete_voltage = 0;
	Position position_received;
	Msg msg_received;
	
	int motor_position = 0;
	int motor_position_past = 0;
	int motor_speed = 0;
	
	
	while(1)
	{
		
		
		
		//printf("K_i: %d\r\n", (int)(K_i*SCALING_FACTOR));
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
		
		//motor_move_dc(position_received);
		score += check_if_scored();
		
		//motor_move_servo((((float)position_received.y) * 1.2 / 200.0) + 1.5); //Maps -100,100 to 0.9,2.1
		//printf("x:%4d y:%4d z:%4d\r", position_received.x,position_received.y,position_received.z);
		//printf("SCORE = %d\r\n", score);
		
		if (position_received.z == 1)
		{
			solenoid_kick();
		}
		

		
		//printf("%d\r\n",motor_encoder_read());
		//discrete_voltage = pid_Controller(0, motor_encoder_read(), &pid);
		//discrete_voltage = discrete_voltage;
		//printf("Volt from pid: %d\r\n", discrete_voltage);
		//motor_move_dc(discrete_voltage);
		//_delay_ms(50);
		
		
		
	
		if (gFlags.pid_timer == 1) {
			motor_position_past = motor_position;
			motor_position = motor_encoder_read();
			motor_speed = motor_position_past - motor_position;
			discrete_voltage = pid_Controller(position_received.y*5, motor_speed, &pid); //position_received.y*40
			
			//printf("ye\r\n");
			//discrete_voltage = pid_Controller(position_received.y*40, motor_encoder_read(), &pid); //position_received.y*40
			//printf("Discrete voltage: %d\r\n, discrete_voltage);
			//discrete_voltage = discrete_voltage;
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
			//printf("Discrete voltage: %d\r\n", discrete_voltage);
			printf("Y= %d\r\n",position_received.y);
			printf("disc= %d\r\n",discrete_voltage);
			motor_move_dc(discrete_voltage);
			//printf("Discrete voltage: %d\r\n", discrete_voltage);
			gFlags.pid_timer = 0;
		}
		_delay_ms(50);
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




/*--TODO
"Create a driver for the timer/counter module which allows you to use the pwm functionality (fast-pwm).
If you also implement the timer interrupt it might save you time when implementing the controller since
most of you would want to use timer interrupt there."

"Create a pwm and/or servo driver which will use your controller output as an input and calculate
the correct duty cycle/on time which you will provide to your time/counter driver. Also implement
safety features which will never let the pwm go out of valid range for the servo."

IMPLEMENT safety features


Automatic? --> "Read values from the IR sensor and print them to the terminal to find the appropriate thresholds settings."

*/

/*--NOTES
Signal period 20 ms

PW of 1.5 ms centers the servo
0.9 ms, minimum
2.1 ms, maximum

1.5 ms +/- 0.6 ms
*/
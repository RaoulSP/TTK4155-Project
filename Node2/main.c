#define F_CPU 16000000
#define NODE_2 2

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "MCP2515.h"
#include "uart.h"
#include "spi.h"
#include "can.h"
#include "mcp.h"
#include "joy.h"
#include "pwm.h"
#include "adc.h"
#include "motor_driver.h"
#include "TWI_Master.h"



int scoring_allowed = 1;
int main(void)
{	
	uart_init(9600, NODE_2);
	spi_master_init();
    mcp_reset();
	can_init(MODE_NORMAL);
	pwm_init();
	adc_init();
	TWI_Master_Initialise();
	motor_init();
	sei(); //fisk
	
	
	int score = 0;


	while(1)
	{
		Position position_received = *(Position*)can_receive();
		printf("x:%4d y:%4d z:%4d\r", position_received.x,position_received.y,position_received.z);
		
		/*
		motor_move_servo((((float)position_received.x)*1.2/200.0)+1.5);
		motor_move_dc(position_received);
		
		
		score += check_if_scored();
		printf("SCORE = %d\r\n", score);
		//printf("%d\r\n", score);
		
		//printf("%d\r\n",adc_read());
		*/
		_delay_ms(50);
	}
}

int check_if_scored()
{
	int trigger_value = 200;
	int adc_val = adc_read();
	//printf("ADC VAL = %d\r\n", adc_val);
	if (adc_val < trigger_value && scoring_allowed){
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
#define NODE_1 1
#define NODE_2 2
#define F_CPU 16000000

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
	printf("\r\n\x1b[4mReset\x1b[0m \r\n");
	adc_init();
	spi_master_init();
    mcp_init();
	can_init(MODE_NORMAL);
	pwm_init();
	TWI_Master_Initialise();
	motor_init();
	sei(); //enable the use of interrupts
	
	int score = 0;

	while(1)
	{
		Position position_received;
		Msg msg_received = can_receive();
		
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
		
		motor_move_dc(position_received);
		score += check_if_scored();
		
		//motor_move_servo((((float)position_received.y) * 1.2 / 200.0) + 1.5); //Maps -100,100 to 0.9,2.1
		//printf("x:%4d y:%4d z:%4d\r", position_received.x,position_received.y,position_received.z);
		//printf("SCORE = %d\r\n", score);
		
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
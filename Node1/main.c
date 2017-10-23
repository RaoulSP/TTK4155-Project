#define F_CPU 4915200

#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"
#include "adc.h"
#include "touch.h"
#include "joy.h"
#include "oled.h"
#include "sram.h"
#include "menu.h"
#include "spi.h"
#include "mcp.h"
#include "can.h"
#include "MCP2515.h"

/*--NOTES--
1 output, 0 input

JTAG interface with the 168
Left to right (row 41-44)
1,5,3,9
Vcc 4
GND 2

char test[] = "raoul&he"; DO NOT USE THIS
char* test = "raoul&he"; USE THIS. Adds \0 termination automatically

printf("%30s",string); Variable string will take fixed amount of space by padding with spaces
printf("%4d",integer); Variable digit will take fixed amount of space by padding with spaces

printf("\b"); Backspace
printf("\n"); Line Feed
printf("\r"); Carriage Return
Only works properly in putty
\r\n is a proper new line! This combination works in Putty, Bray's and Termite
*/

int main(void)
{
	MCUCR |= (1<<SRE);		//Enable external memory
	SFIOR |= (1<<XMM2);
	
	uart_init(9600);
	adc_init();
	oled_init();
	oled_print_string("Please calibrate joystick!",0,0,8,0);
	oled_refresh();
	joy_init();
	touch_init();
	menu_init();
	spi_master_init();
	mcp_init();
	can_init(MODE_NORMAL); 
	oled_clear_screen();
	oled_refresh();
	
	/*
	char k = (spi_master_transmit('a'));
	printf("%d",k);
	char = (spi_master_transmit('a'));
	printf("%d",z);
	*/
	
	//mcp_request_to_send(0,1,1);
	//printf("Read status: %d\n", mcp_read_status());
	//char STREAING[13] = "Hans er sist!";
	//mcp_write(0x1, STREAING, 13);
	//char* arr = (mcp_read(0x1,13));
	//printf("%s\n",arr);
	
	while (1)
	{
		Position position = joy_get_position();
		int position_length = sizeof(position);
		can_write(42, (char *)&position, position_length);
		/*
		//for loopback mode:
		Position* position2_ptr = (Position*)can_read(); //?
		Position position2 = *position2_ptr;
		Position position2 = *(Position*)can_read(); //single line
		printf("x:%4d\ty:%4d\r",position2.x,position2.y);
		*/
		menu_run_display();
		_delay_ms(100);	
	}
	
	/*--TODO--
	-Add a brightness function
	-Add a interrupt for 60 Hz updates

	-can_transmitt(msg);
	can_msg_t msg = can_receive();
	position3.x = msg.data[0];

	Tips from student assistant:
	sleep(); avr/sleep.h? Don't run main loop more often than necessary
	Read about various sleep states. Sleep state for ADC reading?
	EEPROM Usage?
	Use fprintf and set up multiple streams
	fprintf(&uart_print,"",string);
	*/
}
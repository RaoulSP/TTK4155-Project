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

/*--TODO--
-Add a brightness function
-Add a interrupt for 60 Hz updates
*/

/*
JTAG interface with the 168

Left to right
1,5,3,9

Vcc 4
GND 2
*/

//1 output, 0 input

int main(void)
{
	MCUCR |= (1<<SRE);		//Enable external memory
	SFIOR |= (1<<XMM2);
	
	//Initialization
	uart_init(9600);
	adc_init();
	oled_init();
	oled_print_string("Please calibrate joystick!",0,0,8,0);
	oled_refresh();
	//joy_init();
	touch_init();
	menu_init();
	spi_master_init();
	mcp_reset(); //reset = init
	can_init(3); //3 for loopback mode
	
	oled_clear_screen();
	oled_refresh();
	
	/*
	char k = (spi_master_transmit('a'));
	printf("%d",k);
	char z = (spi_master_transmit('a'));
	printf("%d",z);
	*/
	
	mcp_request_to_send(0,1,1);
	printf("Read status: %d\n", mcp_read_status());
	char STREAING[13] = "Hans er sist!";
	mcp_write(0x1, STREAING, 13);
	char* arr = (mcp_read(0x1,13));
	printf("%s\n",arr);
	
	while (1)
	{
		//printf("\n");
		menu_run_display();
		
		//SPDR = 0b11111111;
		//_delay_ms(10);
		
	}

}

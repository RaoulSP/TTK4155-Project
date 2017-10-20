#define F_CPU 16000000

#include <avr/io.h>
#include "MCP2515.h"
#include "uart.h"
#include "spi.h"
#include "can.h"
#include "mcp.h"
#include <util/delay.h>

int main(void)
{	
	uart_init(9600);
	spi_master_init();
    mcp_reset();
	can_init(MODE_NORMAL);
	while(1)
    {
		//printf("%s\n",can_read());
		char* position = can_read();
		int x = position[0];
		int y = position[1];

		
		
		printf("%d\t %d \n",x,y);
		//printf("hey");

		_delay_ms(1000);
    }
}
#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

#include "MCP2515.h"
#include "uart.h"
#include "spi.h"
#include "can.h"
#include "mcp.h"
#include "joy.h"

int main(void)
{	
	uart_init(9600);
	spi_master_init();
    mcp_reset();
	can_init(MODE_NORMAL);
	
	while(1)
    {
		Position position2 = *(Position*)can_receive();
		printf("x:%4d\ty:%4d\tz:%4d\r",position2.x,position2.y,position2.z);
		_delay_ms(100);
    }
}
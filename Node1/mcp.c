#include <avr/io.h>

#include "spi.h"
#include "MCP2515.h"

char mcp_read(char address){
	clear_bit(PORTB, PB4);
	spi_master_transmit(MCP_READ);
	spi_master_transmit(address);
	char data = spi_master_transmit(0x0); //0x0 = dummy value
	set_bit(PORTB,PB4);
	return data;
}

void mcp_write(char address, char data){
	clear_bit(PORTB, PB4);
	spi_master_transmit(MCP_WRITE);
	spi_master_transmit(address);
	spi_master_transmit(data);
	set_bit(PORTB,PB4);
}

void mcp_request_to_send(int TXB2, int TXB1, int TXB0){
	int command = 0b10000000 | TXB0 | (TXB1 << 1) | (TXB2 << 2);
	clear_bit(PORTB, PB4);
	spi_master_transmit(command);
	set_bit(PORTB, PB4);
}

char mcp_read_status(){
	clear_bit(PORTB, PB4);
	spi_master_transmit(MCP_READ_STATUS);
	char status = spi_master_transmit(0x00);
	set_bit(PORTB ,PB4);
	return status;
}

void mcp_bit_modify(char address, char mask, char data){
	clear_bit(PORTB, PB4);
	spi_master_transmit(MCP_BITMOD);	
	spi_master_transmit(address);
	spi_master_transmit(mask);
	spi_master_transmit(data);
	set_bit(PORTB, PB4);
}
		
void mcp_reset(){
	clear_bit(PORTB, PB4);
	spi_master_transmit(MCP_RESET);	
	set_bit(PORTB, PB4);
}
	
void mcp_init(){
	mcp_reset();
}

int mcp_test(){
	mcp_request_to_send(1,1,1);
	int result = mcp_read_status();
	printf("Testing MCP, expected %d, should be 84", result); //(1,1,1) should print 64 + 16 + 4 = 84
	if (result == 84){
		printf("\t...OK\r\n");
		return 0;
	}
	else{
		printf("\t...NOT OK\r\n");
		return 1;
	}
}

/*Testing code taken from main.c - this test doesn't work correctly, even when SPI and MCP are working.
	char* STREAING = "Hans er sist!";
	mcp_write(0x1, STREAING, 13);
	char* arr = (mcp_read(0x1), 13);
	printf("%s\n",arr);
*/
#include "settings.h"
#include "spi.h"
#include "mcp.h"

char mcp_read(char address){
	clear_bit(PORTB, MCP_PIN);
	spi_master_transmit(MCP_READ);
	spi_master_transmit(address);
	char data = spi_master_transmit(0x0); //0x0 = dummy value
	set_bit(PORTB,MCP_PIN);
	return data;
}

void mcp_write(char address, char data){
	clear_bit(PORTB, MCP_PIN);
	spi_master_transmit(MCP_WRITE);
	spi_master_transmit(address);
	spi_master_transmit(data);
	set_bit(PORTB,MCP_PIN);
}

void mcp_request_to_send(int TXB2, int TXB1, int TXB0){
	int command = 0b10000000 | TXB0 | (TXB1 << 1) | (TXB2 << 2);
	clear_bit(PORTB, MCP_PIN);
	spi_master_transmit(command);
	set_bit(PORTB, MCP_PIN);
}

char mcp_read_status(){
	clear_bit(PORTB, MCP_PIN);
	spi_master_transmit(MCP_READ_STATUS);
	char status = spi_master_transmit(0x00);
	set_bit(PORTB ,MCP_PIN);
	return status;
}

void mcp_bit_modify(char address, char mask, char data){
	clear_bit(PORTB, MCP_PIN);
	spi_master_transmit(MCP_BITMOD);	
	spi_master_transmit(address);
	spi_master_transmit(mask);
	spi_master_transmit(data);
	set_bit(PORTB, MCP_PIN);
}
		
void mcp_reset(){
	clear_bit(PORTB, MCP_PIN);
	spi_master_transmit(MCP_RESET);
	set_bit(PORTB, MCP_PIN);
}
	
void mcp_init(){
	spi_master_init();
	mcp_reset();
}
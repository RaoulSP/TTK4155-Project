#include "spi.h"
#include <avr/io.h>

char* mcp_read(char address, int bytes){
	char ret[bytes];
	clear_bit(PORTB, PB4);
	spi_master_transmit(0b00000011);
	spi_master_transmit(address);
	for (int i = 0; i < bytes; i++)
	{
		ret[i] = spi_master_transmit(0x00);
	}
	set_bit(PORTB,PB4);
	return ret;
}

void mcp_write(char address, char* data, int length){
	clear_bit(PORTB, PB4);
	spi_master_transmit(0b00000010);
	spi_master_transmit(address);
	for (int i = 0; i < length; i++)
	{
		spi_master_transmit(data[i]);
	}
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
	spi_master_transmit(0b10100000);
	char status = spi_master_transmit(0x00);
	set_bit(PORTB ,PB4);
	return status;
}

void mcp_bit_modify(char address, char mask, char data){
	clear_bit(PORTB, PB4);
	spi_master_transmit(0b00000101);	
	spi_master_transmit(address);
	spi_master_transmit(mask);
	spi_master_transmit(data);
	set_bit(PORTB, PB4);
}
		
void mcp_reset(){
	clear_bit(PORTB, PB4);
	spi_master_transmit(0b11000000);	
	set_bit(PORTB, PB4);
	} //call on init?

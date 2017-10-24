#include <avr/io.h>

#include "mcp.h"
#include "can.h"
#include "MCP2515.h"

void can_init(int mode){
	//priority bits? TXP
	//CANINTE.TXInE
	mcp_bit_modify(0x2B,0b00011100, 0b00000000); //Disables interrupts from empty buffers
	mcp_bit_modify(MCP_RXB0CTRL, 0b01100000, 0b01100000); //Disable filters
	mcp_bit_modify(MCP_CANCTRL, 0xFF, mode); //Sets operation mode 
	
	//TODO: Filter initialization
}

void can_transmit(int id, char *data, int data_length){
	//TXB0Dm - Data transmit buffer - The bytes with all the data
	mcp_write(0x41, id >> 3);  //TXB0SIDH - Higher identifier byte
	mcp_write(0x42, id << 5);  //TXB0SIDL - Lower identifier byte
	mcp_write(0x45, data_length); //TXB0DLC	- Data length - RTR will be set to 0 (Data Frame message)
	for(int i = 0; i < data_length; i++){
		mcp_write(0x46 + i, data[i]); 
	}
	mcp_request_to_send(0,1,0);	//Request to send - sending will start as soon as the bus is clear
	
	//TODO: use can_msg_t struct containing entire frame
	//TODO: (maybe) print error message? TXBnCTRL.TXERR and the CANINTF.MERRF bits will be set and an interrupt will be generated on the INT pin if the CANINTE.MERRE bit is set
}

//MCP_TXB0CTRL?
char * can_receive(){
	uint8_t data_length = mcp_read(0x65) & 0b00001111;
	char data[data_length];
	for(int i = 0; i < data_length; i++){
		data[i] = mcp_read(0x66 + i);  //Read data
	}
	mcp_bit_modify(0x2C, 1, 0);	//CANINTF - Sets RX0IF to 0
	return data;
	
	//TODO: Add something to read the standard identifier of the message received
}
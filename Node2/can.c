#include <avr/io.h>
#include <util/delay.h>

#include "mcp.h"
#include "can.h"
#include "MCP2515.h"

void can_init(int mode){
	mcp_bit_modify(0x2B,0b00011100, 0b00000000); //Disables interrupts from empty buffers
	mcp_bit_modify(MCP_RXB0CTRL, 0b01100000, 0b01100000); //Disable filters
	mcp_bit_modify(MCP_CANCTRL, 0xFF, mode); //Sets operation mode
	
	//TODO: Filter initialization
	//NOTES: CANINTE.TXInE
	//priority bits? TXP
}

void can_transmit(Msg msg){
	int buffer = 2; //n = 1, 2 or 3
	
	mcp_write(0x31 + 0x10*buffer, msg.id >> 3);  //TXBnSIDH
	mcp_write(0x32 + 0x10*buffer, msg.id << 5);  //TXBnSIDL
	mcp_write(0x35 + 0x10*buffer, msg.length); //TXBnDLC	//RTR will be set to 0 (Data Frame message)
	
	for(int i = 0; i < msg.length; i++){
		mcp_write(0x36 + 0x10*buffer + i, msg.data[i]); //TXBnDm
	}
	
	mcp_request_to_send(buffer == 2, buffer == 1, buffer == 0);	//Request to send - sending will start as soon as the bus is clear
	
	//TODO: (maybe) print error message?
	//TXBnCTRL.TXERR and the CANINTF.MERRF bits will be set and an interrupt will be generated on the INT pin if the CANINTE.MERRE bit is set
}

Msg can_receive(){
	int buffer = 0; //n = 1 or 2
	
	Msg msg;
	msg.id = ((int)mcp_read(0x61 + 0x10*buffer) << 3) | (mcp_read(0x62 + 0x10*buffer) >> 5); //Put together RXBnSIDH and RXBnSIDL
	msg.length = mcp_read(0x65 + 0x10*buffer) & 0b00001111; //RXBnDLC
	msg.data = malloc(msg.length);
	
	for(int i = 0; i < msg.length; i++){
		msg.data[i] = mcp_read(0x66 + 0x10*buffer + i);
	}
	
	mcp_bit_modify(0x2C, buffer + 1, 0);	//CANINTF - Sets RX0IF to 0
	return msg;
}

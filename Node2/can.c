#include "mcp.h"
#include "can.h"
#include "MCP2515.h"
#include <avr/io.h>

void can_init(int mode){
	//priority bits? TXP
	//CANINTE.TXInE
	//Disables interrupts from empty buffers
	mcp_bit_modify(0x2B,0b00011100, 0b00000000);
	//Disable filters
	mcp_bit_modify(MCP_RXB0CTRL, 0b01100000, 0b01100000);
	//TODO: Filter initialization
	//Sets operation mode
	mcp_bit_modify(MCP_CANCTRL, 0xFF, mode);
	
}

//can_msg_t

void can_write(int id, char *data, int data_length){
	//int data_length = strlen(data);
	mcp_write(0x41, id >> 3);  //TXB0SIDH - Higher identifier byte
	mcp_write(0x42, id << 5);  //TXB0SIDL - Lower -------||-------
	mcp_write(0x45, data_length); //TXB0DLC	- Data length - RTR will be set to 0 (Data Frame message)
	//TXB0Dm - Data transmit buffer - The bytes with all the data
	for(int i = 0; i < data_length; i++){
		mcp_write(0x46 + i, data[i]);
	}
	mcp_request_to_send(0,1,0);		//Request to send - sending will start as soon as the bus is clear
	
	/*print error message? TXBnCTRL.TXERR and the CANINTF.MERRF bits will be set and an
	interrupt will be generated on the INT pin if the CANINTE.MERRE bit is set
	*/
}

//MCP_TXB0CTRL
char* can_receive(){
	//Add something to read the standard identifier of the message received
	
	uint8_t data_length = mcp_read(0x65) & 0b00001111;
	char data[data_length];
	for(int i = 0; i < data_length; i++){
		data[i] = mcp_read(0x66 + i);  //Read data
	}
	//data[data_length] = '\0';
	mcp_bit_modify(0x2C, 1, 0);	//CANINTF - Sets RX0IF to 0
	return data;
}
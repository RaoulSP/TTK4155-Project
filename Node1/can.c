#include <avr/io.h>
#include <util/delay.h>

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

void can_transmit(Msg msg){
	//TXB0Dm - Data transmit buffer - The bytes with all the data
	mcp_write(0x41, msg.id >> 3);  //TXB0SIDH - Higher identifier byte
	mcp_write(0x42, msg.id << 5);  //TXB0SIDL - Lower identifier byte
	mcp_write(0x45, msg.length); //TXB0DLC	- Data length - RTR will be set to 0 (Data Frame message)
	for(int i = 0; i < msg.length; i++){
		mcp_write(0x46 + i, msg.data[i]);
	}
	mcp_request_to_send(0,1,0);	//Request to send - sending will start as soon as the bus is clear
	
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
	//use Msg type? Return Msg type and access data using "can_receive().data"?
}

int can_test(){ 
	mcp_bit_modify(MCP_CANCTRL, 0xFF, MODE_LOOPBACK);
	char* test_string = "test123";
	Msg msg;
	msg.id = 42;
	msg.length = sizeof(test_string);
	msg.data = test_string;
	can_transmit(msg);
	printf("Sent '%s'... ", test_string);
	_delay_ms(10);
	printf("received '%s'\r\n",can_receive());
	return 0; 
	
	//TODO: Make it work. Currently receiving nothing
	//Possibly add logic to return 1
	//mcp_bit_modify back to the original CAN_MODE
}
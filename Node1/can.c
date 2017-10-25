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
	int buffer = 2;
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

char* can_receive(){
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




//Versions of can_transmit:

/*
void can_transmit(Msg msg){
	int remaining = msg.length;
	while(remaining){
		if (remaining > 8){
			msg.length = 8;
		}
		else{
			msg.length = remaining;
		}
		mcp_write(0x41, msg.id >> 3); //TXBnSIDH
		mcp_write(0x42, msg.id << 5); //TXBnSIDL
		mcp_write(0x45, msg.length); //TXBnDLC			//RTR will be set to 0 (Data Frame message)
		for(int j = 0; j < msg.length; j++){
			mcp_write(0x46 + j, msg.data[j]); //TXBnDm
		}
		mcp_request_to_send(0,1,0);	//Request to send - sending will start as soon as the bus is clear
		remaining -= msg.length;
		msg.data += 8;
	}
	//TODO: (maybe) print error message? TXBnCTRL.TXERR and the CANINTF.MERRF bits will be set and an interrupt will be generated on the INT pin if the CANINTE.MERRE bit is set
}



void can_transmit(Msg msg){
	for (int i = 0; i < 1 + (msg.length - 1)/8; i++){
		mcp_write(0x41, msg.id >> 3); //TXBnSIDH
		mcp_write(0x42, msg.id << 5); //TXBnSIDL
		mcp_write(0x45, msg.length); //TXBnDLC			//RTR will be set to 0 (Data Frame message)
		for(int j = 0; j < msg.length; j++){
			mcp_write(0x46 + j, msg.data[j]); //TXBnDm
		}
		mcp_request_to_send(0,1,0);	//Request to send - sending will start as soon as the bus is clear
	}
	//TODO: (maybe) print error message? TXBnCTRL.TXERR and the CANINTF.MERRF bits will be set and an interrupt will be generated on the INT pin if the CANINTE.MERRE bit is set
}



void can_transmit(Msg msg){
	int length_remaining = msg.length;
	Msg msg_part;
	msg_part.id = msg.id;
	msg_part.length = 8;
	msg_part.data = msg.data;
	
	while (length_remaining > 8){
		can_transmit(msg_part);
		length_remaining -= 8;
		msg_part.data += 8;
	}
	
	msg_part.length = length_remaining;
	mcp_write(0x41, msg.id >> 3);  //TXB1SIDH - Higher identifier byte
	mcp_write(0x42, msg.id << 5);  //TXB1SIDL - Lower identifier byte
	mcp_write(0x45, msg_part.length); //TXB1DLC	- Data length - RTR will be set to 0 (Data Frame message)
	for(int i = 0; i < msg_part.length; i++){
		mcp_write(0x46 + i, msg_part.data[i]); //TXB1Dm - Data transmit buffer - The bytes with all the data
	}
	mcp_request_to_send(0,1,0);	//Request to send - sending will start as soon as the bus is clear
	//TODO: (maybe) print error message? TXBnCTRL.TXERR and the CANINTF.MERRF bits will be set and an interrupt will be generated on the INT pin if the CANINTE.MERRE bit is set
}
	
	
	
void can_transmit(Msg msg){	
	if (msg.length > 8){
		int parts = msg.length/8;
		int remainder = msg.length % 8;
		for (int i = 0; i < parts; i++){ //piece, section, chunk, part, frame, partial_msg?
			Msg msg_part;
			msg_part.id = msg.id;
			msg_part.length = 8;
			msg_part.data += i*8;
			can_transmit(msg_part);
		}
		if (remainder){
			//????
		}
	}
	else{
		mcp_write(0x41, msg.id >> 3);  //TXB1SIDH - Higher identifier byte
		mcp_write(0x42, msg.id << 5);  //TXB1SIDL - Lower identifier byte
		mcp_write(0x45, msg.length); //TXB1DLC	- Data length - RTR will be set to 0 (Data Frame message)
		for(int i = 0; i < msg.length; i++){
			mcp_write(0x46 + i, msg.data[i]); //TXB1Dm - Data transmit buffer - The bytes with all the data
		}
		mcp_request_to_send(0,1,0);	//Request to send - sending will start as soon as the bus is clear
	}
	//TODO: (maybe) print error message? TXBnCTRL.TXERR and the CANINTF.MERRF bits will be set and an interrupt will be generated on the INT pin if the CANINTE.MERRE bit is set
}



void can_transmit(Msg msg){
	mcp_write(0x41, msg.id >> 3);  //TXB1SIDH - Higher identifier byte
	mcp_write(0x42, msg.id << 5);  //TXB1SIDL - Lower identifier byte
	mcp_write(0x45, msg.length); //TXB1DLC	- Data length - RTR will be set to 0 (Data Frame message)
	for(int i = 0; i < msg.length; i++){
		mcp_write(0x46 + i, msg.data[i]); //TXB1Dm - Data transmit buffer - The bytes with all the data
	}
	mcp_request_to_send(0,1,0);	//Request to send - sending will start as soon as the bus is clear
	
	//TODO: (maybe) print error message? TXBnCTRL.TXERR and the CANINTF.MERRF bits will be set and an interrupt will be generated on the INT pin if the CANINTE.MERRE bit is set
}
*/
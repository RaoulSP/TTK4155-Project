#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "can.h"
#include "settings.h"
#include "interrupts.h"
volatile int can_message_received = 0;

void can_init(int mode){
	mcp_init();
	
	mcp_bit_modify(0x2B,0b00000011, 0b00000011); //enable receive msg interrupt
	mcp_bit_modify(MCP_RXB0CTRL, 0b01100000, 0b01100000); //Disable filters
	mcp_bit_modify(MCP_CANCTRL, 0xFF, mode); //Sets operation mode 
	//TODO: Filter initialization
	//NOTES: CANINTE.TXInE
	//priority bits? TXP
	
	#ifdef NODE_1
	//External interrupt
	MCUCR |= (1 << ISC01);
	GICR |= (1 << INT0);
	#endif
}

void can_transmit(Msg msg){
	int buffer = 2; //n = 1, 2 or 3, buffer = n - 1
	
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
	int buffer = 0; //n = 1 or 2, buffer = n - 1
	
	Msg msg;
	msg.id = ((int)mcp_read(0x61 + 0x10*buffer) << 3) | (mcp_read(0x62 + 0x10*buffer) >> 5); //Put together RXBnSIDH and RXBnSIDL
	msg.length = mcp_read(0x65 + 0x10*buffer) & 0b00001111; //RXBnDLC
	msg.data = malloc(msg.length);
	
	for(int i = 0; i < msg.length; i++){
		msg.data[i] = mcp_read(0x66 + 0x10*buffer + i);
	}
	
	mcp_bit_modify(0x2C, buffer + 1, 0); //CANINTF - Sets RXnIF to 0
	return msg;
}

Msg can_construct_msg(int id, int length, char* data){
	Msg msg = {id,length,data};
	return msg;
} 

ISR(INT0_vect){
	can_message_received = 1;
	//printf("ISR: %d\r\n", can_message_received);
}


//void can_test(){ 
	//mcp_bit_modify(MCP_CANCTRL, 0xFF, MODE_LOOPBACK);
	//char* test_string = "test123";
	//Msg msg_test;
	//msg_test.id = 1;
	//msg_test.length = (strlen(test_string) + 1);
	//msg_test.data = test_string;
	//can_transmit(msg_test);
	//
	//char* received_string = can_receive().data;
	//printf("CAN: Sent '%s'... received '%s'\r\n", test_string, received_string);
	//free(received_string);
	//
	////TODO: Make it work. Currently receiving nothing
	////Possibly add logic to return 1
	////mcp_bit_modify back to the original CAN_MODE
//}

/*
void can_receive_at_location(char* write_location){
	int buffer = 0; //n = 1 or 2
	
	Msg msg;
	msg.id = ((int)mcp_read(0x61 + 0x10*buffer) << 3) | (mcp_read(0x62 + 0x10*buffer) >> 5); //Put together RXBnSIDH and RXBnSIDL
	msg.length = mcp_read(0x65 + 0x10*buffer) & 0b00001111; //RXBnDLC
	msg.data = write_location;
	
	for(int i = 0; i < msg.length; i++){
		msg.data[i] = mcp_read(0x66 + 0x10*buffer + i);
	}
	
	mcp_bit_modify(0x2C, buffer + 1, 0);	//CANINTF - Sets RX0IF to 0
}

void can_receive_and_switch(){
	int buffer = 0; //n = 1 or 2
	//char* string_location = NULL;
	
	Msg msg;
	msg.id = ((int)mcp_read(0x61 + 0x10*buffer) << 3) | (mcp_read(0x62 + 0x10*buffer) >> 5); //Put together RXBnSIDH and RXBnSIDL
	msg.length = mcp_read(0x65 + 0x10*buffer) & 0b00001111; //RXBnDLC
	switch(msg.id){
		case 1:
			//msg.data = ;//address to some string?
		case 11:
			//msg.data = //address to some struct?
			break;
		case 42:
			//msg.data = //address to some int? etc.
			break;
		case 666:
			msg.data = malloc(msg.length);
			string_location = msg.data;
			break;
		case 667:
			msg.data = malloc(msg.length);
			for(int i = 0; i < msg.length; i++){
				msg.data[i] = mcp_read(0x66 + 0x10*buffer + i);
			}
			printf("%s",msg.data);
			free(msg.data);
			break;
		default:
			printf("unknown CAN msg received");
			mcp_bit_modify(0x2C, buffer + 1, 0);
			return;
	}
	
	for(int i = 0; i < msg.length; i++){
		msg.data[i] = mcp_read(0x66 + 0x10*buffer + i);
	}
	
	//free(string_location);
	//printf("%s",msg.data);
	
	mcp_bit_modify(0x2C, buffer + 1, 0);	//CANINTF - Sets RX0IF to 0
}
*/


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
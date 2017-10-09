#include "mcp.h"



void can_init(int mode){
	//priority bits? TXP
	//CANINTE.TXInE?
	
	//Disables interrupts from empty buffers
	mcp_bit_modify(0x2B,0b00011100, 0b00000000);
	



	//Sets operation mode 
	mcp_bit_modify(0x0F, 0b00000111, mode);
}

void can_write(Frame frame){
	//send data to buffer?
	mcp_request_to_send(0,0,0);
	//send data to buffer?
	
	/*print error message? TXBnCTRL.TXERR and the CANINTF.MERRF bits will be set and an
	interrupt will be generated on the INT pin if the CANINTE.MERRE bit is set
	*/
}
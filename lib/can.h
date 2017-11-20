#ifndef CAN_H_
#define CAN_H_

#include "mcp.h"

typedef struct can_msg_t {
	int id;
	int length;
	char* data;
} Msg;

typedef enum can_id_t {
	COMMAND, //can ask for status, "on/off and calibrated or not"
	STATUS,
	STRING,
	OCCLUDED,
	POSITION
} Can_id;

void can_init(int mode);
void can_transmit(Msg msg); 
Msg can_receive();
Msg can_construct_msg(int id, int length, char* data);

//void can_test();
//void can_receive_at_location(char* write_location);
//void can_receive_and_switch();

#endif /* CAN_H_ */
#ifndef CAN_H_
#define CAN_H_

typedef struct can_msg_t {
	int id;
	int length;
	char* data;
} Msg;

void can_init(int mode);
void can_transmit(Msg msg); 
Msg can_receive();
void can_test();

void can_receive_at_location(char* write_location);
void can_receive_and_switch();

#endif /* CAN_H_ */
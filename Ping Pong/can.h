

#ifndef CAN_H_
#define CAN_H_


typedef struct message_t {
	int id;
	char data;
	int length;	
}Message;


void can_init(int mode);
void can_write();




#endif /* CAN_H_ */
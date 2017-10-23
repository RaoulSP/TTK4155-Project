

#ifndef CAN_H_
#define CAN_H_

typedef struct frame_t {
	int id;
	char data;
	int length;
}Frame;
//can_msg_t

void can_init(int mode);
void can_write(int id, char *data, int data_length);
char* can_read();



#endif /* CAN_H_ */
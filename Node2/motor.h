

#ifndef MOTOR_H_
#define MOTOR_H_



void motor_init();
void motor_encoder_reset();
int motor_encoder_read();
void motor_move_with_pid(int position);
void motor_move(int discrete_voltage);


#endif /* MOTOR_H_ */
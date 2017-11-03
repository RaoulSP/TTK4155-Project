

#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_



void motor_init();
void motor_move_dc(Position pos);
void motor_move_servo(float ms);

#endif /* MOTOR_DRIVER_H_ */
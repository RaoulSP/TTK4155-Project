

#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_



void motor_init();
void motor_encoder_reset();
int motor_encoder_read();
void motor_move_dc_with_pid(int position);
void motor_move_dc(int discrete_voltage);
void motor_move_servo(float ms);

#endif /* MOTOR_DRIVER_H_ */
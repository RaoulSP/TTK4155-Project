#ifndef PWM_H_
#define PWM_H_

void pwm_init();
void pwm_set_duty_cycle(float ms);
void pwm_change_freq(int freq);

#endif /* PWM_H_ */
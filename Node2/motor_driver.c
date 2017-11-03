#include <avr/io.h>
#include "joy.h"
#include "pwm.h"
void motor_init(){
	DDRH |= (1 << PH4); //Enable motor
	//DDRH |= (1 << PH1); //Direction right (?)
	//PORTB |= ((1 << PH1) | (1 << PH4)); //This sets pull-up R for SCL and SDA. Is this really needed? Ask the studass?
		
}

void motor_move_dc(Position pos){
	if (pos.x >= 50)
	{
		DDRH |= (1 << PH1); 
		int voltage = 0b00000111;
		int dac_address = 0b0101000;
		char msg[2];
		msg[0] = ((dac_address << 1) | 0); //0 for write, 1 for read
		msg[1] = voltage;
		char length = 2;
		TWI_Start_Transceiver_With_Data(msg, length);
	}
	else if (pos.x <= -50)
	{
		DDRH &= ~(1 << PH1);  
		int voltage = 0b00000111;
		int dac_address = 0b0101000;
		char msg[2];
		msg[0] = ((dac_address << 1) | 0); //0 for write, 1 for read
		msg[1] = voltage;
		char length = 2;
		TWI_Start_Transceiver_With_Data(msg, length);
	}
}

void motor_move_servo(float ms){
	if(ms > 2.1){
		ms = 2.1;
	}
	else if (ms < 0.9){
		ms = 0.9;
	}
	pwm_set_duty_cycle(ms);
}

//address: 0101000 DAC

//I2C frame: 
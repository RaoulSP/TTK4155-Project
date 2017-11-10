#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

#include "joy.h"
#include "pwm.h"

//Rename files to motor.c and motor.h?

void motor_init(){
	//MOTOR 
	DDRH |= (1 << PH1); //Set motor direction pin as output
	DDRH |= (1 << DDH4); //Set motor enable pin as output
	PORTH |= (1 << PH4); //Set motor enable pin high
	//PORTB |= ((1 << PH1) | (1 << PH4)); //This sets pull-up R for SCL and SDA. Is this really needed? Ask the studass?	
	
	//ENCODER
	DDRH |= (1 << PH5); //OE as output
	DDRH |= (1 << PH3); //SEL as output
	DDRH |= (1 << PH6); //RST as output
	DDRK = 0b00000000; //Encoder vals as input 
	motor_encoder_reset();
	
}

void motor_encoder_reset() {
	PORTH &= ~(1 << PH6);
	_delay_us(20);
	PORTH |= (1 << PH6);
}



int motor_encoder_read(){
	PORTH &= ~(1 << PH5);
	PORTH &= ~(1 << PH3); //Set sel low to get MSB
	_delay_us(20); //mby?
	int encoder_val = (PINK << 8); //MSB
	PORTH |= (1 << PH3); //Set sel high to get LSB
	_delay_us(20); //mby?
	encoder_val |= PINK;  
	//MBY TOGGLE RESET?
	
	PORTH |= (1 << PH5);
	return encoder_val;
}

void motor_move_dc(int discrete_voltage){
	
	if (discrete_voltage >= 0){
		PORTH |= (1 << PH1); //Set direction
	}
	else if (discrete_voltage < 0){
		PORTH &= ~(1 << PH1); //Set other direction
		discrete_voltage = -discrete_voltage;
	}
	
	int dac_address = 0b0101000;
	char length = 3;
	char msg[3];
	msg[0] = ((dac_address << 1) | 0); //0 = write, 1 = read
	msg[1] = 0b00000000; //Command byte: R2, R1, R0, Rst, PD, A2, A1, A0
	msg[2] = discrete_voltage; //Value of 0-255, maps to 0V-5V
	TWI_Start_Transceiver_With_Data(msg, length);
	
	//Just for fun, no ifs:
	//PORTH |= ((pos.y > 0) << PH1); //Set direction
	//int voltage = (abs(pos.y) > 10) * abs(pos.y); //Set voltage to magnitude of pos.y, or 0
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

//= (uint8_t)pos.y; //& ~(1 << sizeof(pos.y)*8)); //Til minne om Hans sin absoluttverdiberegning
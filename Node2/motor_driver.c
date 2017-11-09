#include <avr/io.h>

#include "joy.h"
#include "pwm.h"

//Rename files to motor.c and motor.h?

void motor_init(){
	DDRH |= (1 << PH1); //Set motor direction pin as output
	DDRH |= (1 << DDH4); //Set motor enable pin as output
	PORTH |= (1 << PH4); //Set motor enable pin high
	//PORTB |= ((1 << PH1) | (1 << PH4)); //This sets pull-up R for SCL and SDA. Is this really needed? Ask the studass?	
}

void motor_move_dc(Position pos){
	int voltage = 0;
	if (pos.y >= 10){
		PORTH |= (1 << PH1); //Set direction
		voltage = pos.y;
	}
	else if (pos.y <= -10){
		PORTH &= ~(1 << PH1); //Set other direction
		voltage = -pos.y;
	}
	
	printf("DAC voltage:%d\r\n",voltage);
	
	int dac_address = 0b0101000;
	char length = 3;
	char msg[3];
	msg[0] = ((dac_address << 1) | 0); //0 = write, 1 = read
	msg[1] = 0b00000000; //Command byte: R2, R1, R0, Rst, PD, A2, A1, A0
	msg[2] = voltage; //Value of 0-255, maps to 0V-5V
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
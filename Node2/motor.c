#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

#include "motor.h"
#include "../lib/joy.h"
#include "../lib/pwm.h"
#include "PID.h"

int motor_position = 0;
int motor_position_past = 0;
int motor_speed = 0;

//initialize PID
pidData_t pid;


void motor_init(){
	//MOTOR 
	DDRH |= (1 << PH1); //Set motor direction pin as output
	DDRH |= (1 << DDH4); //Set motor enable pin as output
	PORTH |= (1 << PH4); //Set motor enable pin high
	
	//ENCODER
	DDRH |= (1 << PH5); //OE as output
	DDRH |= (1 << PH3); //SEL as output
	DDRH |= (1 << PH6); //RST as output
	DDRK = 0b00000000; //Encoder vals as input 
	motor_encoder_reset();
	
	double K_p = 0.075;	//0.075
	double K_d = 0;		//0
	double K_i = 0.00;	//0
	pid_Init(K_p, K_i, K_d, &pid);
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

void motor_move(int discrete_voltage){
	int voltage = 0;
	
	if (discrete_voltage > 255){
		discrete_voltage = 255;
	}
	else if (discrete_voltage < -255){
		discrete_voltage = -255;
	}
	
	if (discrete_voltage >= 0){
		PORTH |= (1 << PH1); //Set direction
		voltage = discrete_voltage;
	}
	else if (discrete_voltage < 0){
		PORTH &= ~(1 << PH1); //Set other direction
		voltage = -discrete_voltage;
	}
	
	int dac_address = 0b0101000;
	char length = 3;
	char msg[3];
	msg[0] = ((dac_address << 1) | 0); //0 = write, 1 = read
	msg[1] = 0b00000000; //Command byte: R2, R1, R0, Rst, PD, A2, A1, A0
	msg[2] = voltage; //Value of 0-255, maps to 0V-5V
	TWI_Start_Transceiver_With_Data(msg, length); //To do: Move some of this code to DAC module?
	
	//Just for fun, no ifs:
	//PORTH |= ((pos.y > 0) << PH1); //Set direction
	//int voltage = (abs(pos.y) > 10) * abs(pos.y); //Set voltage to magnitude of pos.y, or 0
}

void motor_move_with_pid(int position){
	motor_position_past = motor_position;
	motor_position = motor_encoder_read();
	motor_speed = motor_position - motor_position_past; //normalize

	int discrete_voltage = pid_Controller(position*20, motor_speed, &pid);

	motor_move(discrete_voltage);
	//To do: Read timer register to normalize time
}

//= (uint8_t)pos.y; //& ~(1 << sizeof(pos.y)*8)); //Til minne om Hans sin absoluttverdiberegning
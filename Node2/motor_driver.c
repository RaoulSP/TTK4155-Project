#include <avr/io.h>
#include "joy.h"
#include "pwm.h"
void motor_init(){
	DDRH |= (1 << DDH4); //Enable motor pin as output
	DDRH |= (1 << PH1); //Set motor direction as output
	PORTH |= (1 << PH4); //Actually set the value of enable
	//DDRH |= (1 << PH1); //Direction right (?)
	//PORTB |= ((1 << PH1) | (1 << PH4)); //This sets pull-up R for SCL and SDA. Is this really needed? Ask the studass?
		
}

void motor_move_dc(Position pos){
	int voltage = 0; //= (uint8_t)pos.y; //& ~(1 << sizeof(pos.y)*8));
	if (pos.y >= 5)
	{
		PORTH |= (1 << PH1); 
		voltage = pos.y;
	}
	else if (pos.y <= -5)
	{
		PORTH &= ~(1 << PH1);  
		voltage = -pos.y;
	}
	printf("%d\r\n",voltage);
	int dac_address = 0b0101000;
	char msg[3];
	msg[0] = ((dac_address << 1) | 0); //0 for write, 1 for read
	msg[1] = 0b00000000; //R2,R1,R0,Rst,PD,A2,A1,A0, (ACK)
	msg[2] = voltage;
	char length = 3;
	TWI_Start_Transceiver_With_Data(msg, length);
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
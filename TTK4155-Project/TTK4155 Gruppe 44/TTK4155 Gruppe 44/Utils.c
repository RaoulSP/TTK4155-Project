
#define F_CPU 4900000
#include <avr/io.h>
#include <util/delay.h>
void TestAle()
{
	//inputs/outputs
	DDRA |= (1<<PA0);
	//Does PE1(ALE) really need to be set to output?
	DDRE |= (1<<PE1);
	
	
	
	PORTA |= (1<<PA0); //PA0 1
	_delay_ms(3000);
	PORTE |= (1<<PE1); //LE 1
	PORTE &= ~(1<<PE1);//LE 0
	_delay_ms(3000);
	PORTA &= ~(1<<PA0); //PA0 0
	_delay_ms(3000);
	PORTE |= (1<<PE1); //LE 1
	PORTE &= ~(1<<PE1);//LE 0
	
	//TODO function which takes a memory address as input and turns on the necessary pins. Separate function for lcd, sram, etc? Then check if the argument address is outside the supposed address space
}
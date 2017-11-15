#ifndef SETTINGS_H_
#define SETTINGS_H_


#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

//#define NODE_1

#ifdef NODE_1 
	#define __AVR_ATmega162__
	#define F_CPU 4915200
	#define MCP_PIN 4
	
#else
	#define __AVR_ATmega2560__
	#define F_CPU 16000000
	#define MCP_PIN 7
#endif


#include <avr/io.h>

#endif /* SETTINGS_H_ */
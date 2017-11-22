/*
 * strings.h
 *
 * Library of strings to store in progmem
 *
 */ 


#ifndef STRINGS_H_
#define STRINGS_H_

#include <avr/pgmspace.h>

const char* PROGMEM strings[95][4] = {
	{0b00000000,0b00000000,0b00000000,0b00000000}, //
	{0b00000000,0b01011100,0b00000000,0b00000000}, // !
	{0b00001100,0b00000000,0b00001100,0b00000000}, // "
};

char string_1[] PROGMEM = "String 1";
char string_2[] PROGMEM = "String 2";
char string_3[] PROGMEM = "String 3";
char string_4[] PROGMEM = "String 4";
char string_5[] PROGMEM = "String 5";

#endif /* STRINGS_H_ */
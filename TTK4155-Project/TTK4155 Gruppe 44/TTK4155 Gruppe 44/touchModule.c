#include <avr/io.h>
#include "utils.h"


void touchModule_init(){
	clear_bit(DDRB, PB0); //Set DDR to input for button
	clear_bit(DDRB, PB1); //Set DDR to input for button
}
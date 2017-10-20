#include <avr/io.h>
#include "spi.h"

void spi_master_init()
{	
	/* Set MOSI, SS and SCK output, all others input */
	//Without setting PB0 as an output printf doesn|t work!?!?!?!?
	DDRB |= (1<<PB1)|(1 << PB2)|(1 << PB7)|(1 << PB0); 	
	//DO NOT TINK IS NECCSCAS RYTSR:
	DDRB &= ~(1 << PB3);

	/* Enable SPI, Master, set clock rate fck/16*/
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);	
	set_bit(PORTB,PB7);
}


char spi_master_transmit(char cData)
{
	/* Start transmission */
	SPDR = cData;	
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));	
	return SPDR;
}

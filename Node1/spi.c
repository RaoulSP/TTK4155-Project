#include <avr/io.h>
#include "spi.h"

void spi_master_init()
{
	/* Set MOSI and SCK output, all others input */
	DDRB |= (1<<PB5)|(1<<PB7)|(1 << PB4); //DDB4
	DDRB &= ~(1 << DDB6);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPI2X);
	set_bit(PORTB,PB4); //Disable chip
	
	//TODO: combine with spi_master_init() for node 2? Take int node as argument
}


char spi_master_transmit(char cData)
{
	SPDR = cData; //Start transmission
	while(!(SPSR & (1<<SPIF))); //Wait for transmission complete	
	return SPDR;
}


/*Testing code taken from main.c - this test doesn't work correctly, even when SPI is working. (?)
	char k = (spi_master_transmit('a'));
	printf("%c\r\n",k);
	char z = (spi_master_transmit('a'));
	printf("%c\r\n",z);
*/
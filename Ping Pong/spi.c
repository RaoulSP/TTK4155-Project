#include <avr/io.h>
#include "spi.h"

void spi_master_init()
{
	
	/* Set MOSI and SCK output, all others input */
	DDRB |= (1<<PB5)|(1<<PB7)|(1 << PB4); //DDB4
	DDRB &= ~(1 << DDB6);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPI2X);
	set_bit(PORTB,PB4);

}


char spi_master_transmit(char cData)
{
	//clear_bit(PORTB, PB4);
	/* Start transmission */
	SPDR = cData;
	
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	//set_bit(PORTB,PB4);
	
	return SPDR;
}

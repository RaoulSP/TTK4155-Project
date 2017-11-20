#include "settings.h"
#include "spi.h"

void spi_master_init()
{
	#ifdef NODE_1
		DDRB |= (1<<DDB4)|(1<<DDB5)|(1<<DDB7); //Set MOSI and SCK output, all others input
		DDRB &= ~(1 << DDB6);
		SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPI2X); //Enable SPI, Master, set clock rate fck/16
		set_bit(PORTB,PB4); //Disable chip
	#else
		//Without setting PB0 as an output printf doesn|t work!?!?!?!?
		DDRB |= (1<<DDB0)|(1 << DDB1)|(1 << DDB2)|(1 << DDB7); //Set MOSI, SS and SCK output, all others input
		DDRB &= ~(1 << DDB3); //DO NOT TINK IS NECCSCAS RYTSR:
		SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0); //Enable SPI, Master, set clock rate fck/16
		set_bit(PORTB,PB7); //Disable chip
	#endif
}

char spi_master_transmit(char cData)
{
	SPDR = cData; //Start transmission
	while(!(SPSR & (1<<SPIF))); //Wait for transmission complete	
	return SPDR;
}


/*This testing code is taken from main.c - this test doesn't work correctly, even when SPI is working. (?)
	char k = (spi_master_transmit('a'));
	printf("%c\r\n",k);
	char z = (spi_master_transmit('a'));
	printf("%c\r\n",z);
*/
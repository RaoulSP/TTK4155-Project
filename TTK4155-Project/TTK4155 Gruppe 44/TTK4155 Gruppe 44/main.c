#include "uart.h"
#include "utils.h"
#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"
#define F_CPU 4900000


int main(void)
{
	//1 output, 0 input 
	
	
	
	uart_init(9600);
	fdevopen(uart_putchar, uart_getchar);
	
	MCUCR |= (1<<SRE);
	SFIOR |= (1<<XMM2);
	//set interrupt bit to input
	clear_bit(DDRE, PE0);
	while (1)
	{
		printf("x = ");
		ADC_read('x');
		
		printf(",  y = ");
		ADC_read('y');
		printf("\n");
	}
	

	
	
	//SRAM_test();

	/*
	DDRA |= (1<<PA0);
	char x[20]; 
    while(1)
    {			
		
		//PORTA |= (1<<PA0);

		//_delay_ms(100);
		//PORTA &= ~(1<<PA0);

		//_delay_ms(100);
		

		scanf("%s", x);
		//putchar(x);
		
		//_delay_ms(1000);
		
		printf(x);
		printf("\n");
		
	}*/
}
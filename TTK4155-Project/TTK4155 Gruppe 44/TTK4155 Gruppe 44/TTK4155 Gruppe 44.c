#include "Uart.h"
#include "Utils.h"
#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 4900000

int main(void)
{
	//1 output, 0 input 
	
	TestAle();
	
	
	/*
	
	uart_init(9600);
	fdevopen(uart_putchar, uart_getchar);
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
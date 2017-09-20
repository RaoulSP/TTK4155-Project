
#include "uart.h"
#include "utils.h"
#include <avr/io.h>
#define F_CPU 4915200
#include <util/delay.h>
#include "adc.h"
#include "touchModule.h"
#include "joy.h"


int main(void)
{
	//1 output, 0 input
	//enable external memory
	
	MCUCR |= (1<<SRE);
	SFIOR |= (1<<XMM2);
	
	
	uart_init(9600);
	ADC_init();
	touchModule_init();
	JOY_init();
	
	while (1)
	{
		
		printf("x = ");
		printf("%d", ADC_read('x'));
		
		printf(",  y = ");
		printf("%d", ADC_read('y'));
		
		printf(",  Left slider = ");
		printf("%d",ADC_read('l'));

		printf(",  Right slider = ");
		printf("%d", ADC_read('r'));
			
		
		printf(",  Left button= ");
		printf("%d", test_bit(PINB, PB1));
		
		printf(",  Right button= ");
		printf("%d", test_bit(PINB, PB0));
		
		printf(",  Middle button= ");
		printf("%d", test_bit(PINB, PB2));
		
		printf(",  X struct= ");
		printf("%d", JOY_getPosition().x);
		
		printf(",  Y struct= ");
		printf("%d", JOY_getPosition().y);
		printf("\n");
	}

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
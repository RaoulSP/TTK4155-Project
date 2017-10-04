
#include "uart.h"
#include "utils.h"
#include <avr/io.h>
#define F_CPU 4915200
#include <util/delay.h>
#include "adc.h"
#include "touchModule.h"
#include "joy.h"
#include "oled.h"

int main(void)
{
	//1 output, 0 input
	//enable external memory
	
	MCUCR |= (1<<SRE);
	SFIOR |= (1<<XMM2);
	
	uart_init(9600);
	ADC_init();
	oled_init();
	
	oled_print_char('H', 55, 5, 4);
	oled_print_string("Hans, Herman og Raoul sitter paa lab. Lorem ipsum dolor sit amet", 0, 0, 5);
	
	/*
	touchModule_init();
	SRAM_test();
	JOY_init();


	while (1)
	{
		
		//oled_write_command(0xB0);
		//oled_write_command(0x0F);
		//oled_write_command(0x10);
		//oled_write_data(0xF0);
		
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
*/
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
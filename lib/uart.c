#include <stdio.h>
#include "settings.h"
#include "uart.h"

void uart_init(int baudRate){
	int UBRR = ((long)F_CPU/((long)16*baudRate) - 1);
	UBRR0L = UBRR;
    UBRR0H = (UBRR>>8);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); //Enable receiver and transmitter 
	
	#ifdef NODE_1
		UCSR0C = ((1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00));
	#else
		UCSR0C = ((1<<USBS0)|(3<<UCSZ00)); //Set frame format: 2 stop bits, 8 data bits //Asynchronous - No parity (?) //URSEL makes sure we won't access UBRRH during operation, but UCSRC. //URSEL should not be set in node 2
	#endif
	fdevopen(uart_putchar, uart_getchar);
}

void uart_putchar(char c){
	while (!( UCSR0A & (1<<UDRE0))); //Wait for empty transmit buffer
	UDR0 = c; //Put data into buffer, sends the data
}

unsigned char uart_getchar(){
	while (!( UCSR0A & (1<<RXC0)));
	return UDR0;
}
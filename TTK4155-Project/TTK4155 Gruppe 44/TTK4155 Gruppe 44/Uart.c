#include <avr/io.h>
#include <stdio.h>

int uart_putchar(char c){
	//Wait for empty transmit buffer
	while (!( UCSR0A & (1<<UDRE0)));
	//Put data into buffer, sends the data
	UDR0 = c;
	return 1;
}

unsigned char uart_getchar(){
	while (!( UCSR0A & (1<<RXC0)));
	return UDR0;
}



int uart_init(int baudRate){
	int UBRR = ((long)4915200/((long)16*baudRate) - 1);
	//Enable printf
	fdevopen(uart_putchar, uart_getchar);
	//Setting
	UBRR0L = UBRR;
    UBRR0H = (UBRR>>8);
    //Enable receiver and transmitter 
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    //Set frame format: 8data, 2stop bit  
    UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
    return 1;
}



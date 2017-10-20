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
	int UBRR = ((long)16000000/((long)16*baudRate) - 1);
	//Enable printf
	fdevopen(uart_putchar, uart_getchar);
	//Setting
	UBRR0L = UBRR;
	UBRR0H = (UBRR>>8);
	//Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	//Set frame format: 2 stop bit, 8data
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	//Ursel makes sure we won't access UBRRH during operation, but UCSRC
	return 1;
}


//Asynchronous - No parity - 2 stop bits - 8 bit char size
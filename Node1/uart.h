
#ifndef UART_H_
#define UART_H_

void uart_init(int baudRate);
void uart_putchar();
unsigned char uart_getchar();

#endif /* UART_H_ */
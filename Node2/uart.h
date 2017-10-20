
#ifndef UART_H_
#define UART_H_

int uart_init(int baudRate);
int uart_putchar();
unsigned char uart_getchar();

#endif /* UART_H_ */
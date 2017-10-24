#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

#ifndef SPI_H_
#define SPI_H_

void spi_master_init(int node);
char spi_master_transmit(char cData);

#endif /* SPI_H_ */
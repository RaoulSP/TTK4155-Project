#include <avr/io.h>

void sram_test(void)
{
	volatile char *ext_ram = (char *) 0x1C00; // Start address for the SRAM. //This and ext_ram_size are modified from original, now that OLED double buffering is on.
	uint16_t ext_ram_size
	= 0x400;
	uint16_t write_errors
	= 0;
	uint16_t retrieval_errors
	= 0;
	printf("Starting SRAM test...\r\n");
	
	uint16_t seed = rand(); // rand() stores some internal state, so calling this function in a loop will yield different seeds each time (unless srand() is called before this function)
	srand(seed);			// Write phase: Immediately check that the correct value was stored
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retrieved_value = ext_ram[i];
		//printf("Write phase : ext_ram[%4d] = %02X (should be %02X)\n", i,retrieved_value, some_value);
		if (retrieved_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i,retrieved_value, some_value);
			write_errors++;
		}
	}
	
	srand(seed);	// Retrieval phase: Check that no values were changed during or after the writephase
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retrieved_value = ext_ram[i];
		if (retrieved_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\r\n",i, retrieved_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \r\n%4d errors in write phase and\r\n%4d errors in retrieval phase\r\n\n", write_errors, retrieval_errors);
}



//Denne forhindrer at du kan bruke navn på ting
/*
void sram_write(uint16_t address, uint8_t data){
	volatile uint16_t *ext_ram = 0x1C00;
	uint16_t ext_ram_size = 0x400;
	
	if(address > ext_ram_size){
		printf("Outside of memory space");
	}
	else{
		ext_ram[address] = data;	
	}
}
*/
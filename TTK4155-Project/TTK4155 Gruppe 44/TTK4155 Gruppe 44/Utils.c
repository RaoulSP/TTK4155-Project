
#define F_CPU 4900000
#include <avr/io.h>
#include <util/delay.h>
void TestAle()
{
	//inputs/outputs
	DDRA |= (1<<PA0);
	//Does PE1(ALE) really need to be set to output?
	DDRE |= (1<<PE1);
	
	PORTA |= (1<<PA0); //PA0 1
	_delay_ms(3000);
	PORTE |= (1<<PE1); //LE 1
	PORTE &= ~(1<<PE1);//LE 0
	_delay_ms(3000);
	PORTA &= ~(1<<PA0); //PA0 0
	_delay_ms(3000);
	PORTE |= (1<<PE1); //LE 1
	PORTE &= ~(1<<PE1);//LE 0
	//TODO function which takes a memory address as input and turns on the necessary pins. Separate function for lcd, sram, etc? Then check if the argument address is outside the supposed address space
}


void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size
	= 0x800;
	uint16_t write_errors
	= 0;
	uint16_t retrieval_errors
	= 0;
	printf("Starting SRAM test...\n");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		//printf("Write phase : ext_ram[%4d] = %02X (should be %02X)\n", i,retreived_value, some_value);
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i,retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the writephase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n",i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n%4d errors in write phase and\n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}
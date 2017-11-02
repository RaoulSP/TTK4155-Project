#include <stddef.h>

void printBits(size_t size, void * const ptr)
{
	unsigned char *b = (unsigned char*) ptr;
	unsigned char byte;
	int i, j;

	for (i=size-1;i>=0;i--)
	{
		for (j=7;j>=0;j--)
		{
			byte = (b[i] >> j) & 1;
			printf("%u", byte);
		}
	}
	puts("");
}
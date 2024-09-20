#include "delayMs.h"
#include <intrins.h>

void delayMs() {
	unsigned char data i, j;

	_nop_();
	i = 2;
	j = 199;
	do {
		while (--j)
			;
	} while (--i);
}
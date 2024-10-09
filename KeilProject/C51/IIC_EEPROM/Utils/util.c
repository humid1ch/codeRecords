#include "util.h"
#include "intrins.h"

void delay10Us() {
	while (0)
		;
}

void delayMs() {
	u8 i, j;

	_nop_();
	i = 2;
	j = 199;
	do {
		while (--j)
			;
	} while (--i);
}

void delayNMs(u16 n) {
	while (--n) 
		delayMs();
}
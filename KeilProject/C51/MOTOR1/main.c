#include "reg52.h"
#include "intrins.h"

sbit MOTOR_IN = P1^0;

void delayMs() {
	unsigned char i, j;

	_nop_();
	i = 2;
	j = 199;
	do {
		while (--j)
			;
	} while (--i);
}

void delayNMs(unsigned int n) {
	while (--n)
		delayMs();
}

void main() {
	MOTOR_IN = 1;
	delayNMs(5000);
	MOTOR_IN = 0;

	while(1);
	
}
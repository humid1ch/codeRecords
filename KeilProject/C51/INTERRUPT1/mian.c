#include <reg52.h>
#include <intrins.h>

#define INT0_ON  \
	{            \
		EA = 1;  \
		EX0 = 1; \
		IT0 = 0; \
	}

#define INT1_ON  \
	{            \
		EA = 1;  \
		EX1 = 1; \
		IT1 = 0; \
	}

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
	
sbit int0t = P3^2;
sbit int1t = P3^3;
	
void int0() interrupt 0 {
	P2 = 0X00;
	delayNMs(500);
	int0t = 0;
}

void int1() interrupt 2 {
	P2 = 0XFF;
	delayNMs(500);
	int1t = 0;
}


void main() {
    INT0_ON;
	INT1_ON;
	
	while(1) {
		int0t = 1;
		int1t = 1;
	}
}
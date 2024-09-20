#include "delay50Us.h"
#include <intrins.h>

void delay50Us() {
	unsigned char data i;

	_nop_();
	i = 20;
	while (--i)
		;
}
#include <reg52.h>

#define LED P2

void delayMS(unsigned int ms) {
	unsigned int i, j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 123; j++)
			;
}

void main() {
	// unsigned char state = 0xFE; // 用于P2赋值, 设置8个LED灯的状态
	// unsigned char flag = 1;		// flag 为1, 表示LED1 -> LED8流水, 为0, 表示 LED8 -> LED1 流水

	// while (1) {
	// 	if (state == 0xFF) {
	// 		if (flag) {
	// 			state = 0xBF; // 1011 1111, 将要从 LED1 -> LED8 流水
	// 			flag = 0;
	// 		}
	// 		else {
	// 			state = 0xFD; // 1111 1101, 将要从 LED8 -> LED1 流水
	// 			flag = 1;
	// 		}
	// 	}

	// 	LED = state; // 通过state设置8个LED灯的状态
	// 	delayMS(300);
	// 	LED = 0xFF; // 8个LED灯全灭

	// 	if (flag) { // 根据 flag 识别流向
	// 		state <<= 1;
	// 		state |= 0x01;
	// 	}
	// 	else {
	// 		state >>= 1;
	// 		state |= 0x80;
	// 	}
	// }

	
	unsigned char nonState = 0x01; // 用于P2赋值, 设置8个LED灯的状态, 表示 ~LED状态
	unsigned char flag = 1;		// flag 为1, 表示LED1 -> LED8流水, 为0, 表示 LED8 -> LED1 流水

	while (1) {
		if (nonState == 0x00) {
			if (flag) {
				nonState = 0x40; // 0100 0000, 将要从 LED1 -> LED8 流水
				flag = 0;
			}
			else {
				nonState = 0x02; // 0000 0010, 将要从 LED8 -> LED1 流水
				flag = 1;
			}
		}

		LED = ~nonState; // 通过~state设置8个LED灯的状态
		delayMS(300);
		LED = 0xFF; // 8个LED灯全灭

		if (flag) // 根据 flag 识别流向
			nonState <<= 1;
		else
			nonState >>= 1;
	}
	
}
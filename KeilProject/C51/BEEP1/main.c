#include <reg52.h>
#include "delay50Us.h"

#define FEQUENCY_S 2000 // 每秒两千次

sbit BEEP = P2 ^ 5;

// 传参超过 200 建议用 delayMs()
void delay50UsN(unsigned char data n) {
	while (--n)
		delay50Us();
}

// 设置蜂鸣器 500us为一次脉冲, 占空比50%, 即 高250us, 低250us
// 约 2000次脉冲/s
void beepSet500() {
	BEEP = 0;
	delay50UsN(5);
	BEEP = 1;
	delay50UsN(5);
}

void main() {
	unsigned char second = 1;				// 需要脉冲多少秒?
	unsigned int cnt = FEQUENCY_S * second; // 脉冲一定秒数 需要脉冲的次数

	while (--cnt)
		beepSet500();

	// BEEP引脚为P2^5, 与LED灯共用

	// BEEP脉冲结束之后, 如果程序结束, 程序会再次执行, BEEP脉冲会继续, BEEP会一直响
	// 所以 禁止程序结束
	while (1)
		;
}
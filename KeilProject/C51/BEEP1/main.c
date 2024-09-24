#include <reg52.h>
#include "delay50Us.h"
#include "delay10Us.h"

#define FEQUENCY_S 2000 // 每秒两千次

sbit BEEP = P2 ^ 5;

unsigned int frequencys[] = {100, 125, 160, 200, 250, 320, 400, 500, 625, 800, 1000, 1250, 1600, 2000, 2500, 3125, 4000, 5000};

// 传参超过 200 建议用 delayMs()
void delay50UsN(unsigned char data n) {
	while (--n)
		delay50Us();
}

void delay10UsN(unsigned char data n) {
	while (--n)
		delay10Us();
}

// 参数:
//  频率 hz, 传入整数, 范围 100 ~ 5000 (保证被1000000整除)
// 	占空比, 传入 整数部分, 比如 50%, 传入 50
// 	维持事件 s, 传入整数, 范围 1 ~ 255
void beep(unsigned int freq, unsigned char duty, unsigned char second) {
	unsigned int cycle = 1000000 / freq;		   // freq频率下的 周期
	float dutyF = (float)duty / 100.0;			   // 占空比 小数
	unsigned int upDuty = cycle * dutyF;		   // 高电平延迟 比
	unsigned int downDuty = cycle * (1.0 - dutyF); // 低电平延迟 比
	unsigned int cnt = freq * second / 2;		   // second秒下的循环次数

	while (--cnt) {
		BEEP = 0;
		delay10UsN(downDuty / 10);
		BEEP = 1;
		delay10UsN(upDuty / 10);
	}
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

	char i = sizeof(frequencys) / sizeof(frequencys[0]) - 1;
	for (i; i >= 0; i--) {
		beep(frequencys[i], 0.2, 1);
	}

	// BEEP脉冲结束之后, 如果程序结束, 程序会再次执行, BEEP脉冲会继续, BEEP会一直响
	// 所以 禁止程序结束
	while (1)
		;
}
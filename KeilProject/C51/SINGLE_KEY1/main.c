#include "reg52.h"
#include "intrins.h"

// 四个按键, 连接引脚默认为高电平, 按下按键 与地导通, 变为低电平
sbit S_KEY1 = P3 ^ 1;
sbit S_KEY2 = P3 ^ 0;
sbit S_KEY3 = P3 ^ 2;
sbit S_KEY4 = P3 ^ 3;

sbit LED1 = P2 ^ 0;
sbit LED2 = P2 ^ 1;
sbit LED3 = P2 ^ 2;
sbit LED4 = P2 ^ 3;

#define NON_KEY_PUSH 0x00
#define S_KEY1_PUSHED 0x08
#define S_KEY2_PUSHED 0x04
#define S_KEY3_PUSHED 0x02
#define S_KEY4_PUSHED 0x01

typedef void (*smgSelFunc)();

// 8位数码管引脚
#define SMG P0

// 3->8译码器的三个引脚
// 板子上 右边数码管的是较小值
sbit SMG_22 = P2 ^ 2;
sbit SMG_23 = P2 ^ 3;
sbit SMG_24 = P2 ^ 4;

// 8个 1位数码管选择函数
void smg1() {
	SMG_24 = 1;
	SMG_23 = 1;
	SMG_22 = 1;
}
void smg2() {
	SMG_24 = 1;
	SMG_23 = 1;
	SMG_22 = 0;
}
void smg3() {
	SMG_24 = 1;
	SMG_23 = 0;
	SMG_22 = 1;
}
void smg4() {
	SMG_24 = 1;
	SMG_23 = 0;
	SMG_22 = 0;
}
void smg5() {
	SMG_24 = 0;
	SMG_23 = 1;
	SMG_22 = 1;
}
void smg6() {
	SMG_24 = 0;
	SMG_23 = 1;
	SMG_22 = 0;
}
void smg7() {
	SMG_24 = 0;
	SMG_23 = 0;
	SMG_22 = 1;
}
void smg8() {
	SMG_24 = 0;
	SMG_23 = 0;
	SMG_22 = 0;
}

// 数码管选择函数 数组
smgSelFunc smgFuncs[] = {smg1, smg2, smg3, smg4, smg5, smg6, smg7, smg8};

// 板子使用 共阴数码管, 各引脚给高电平点亮边
// a b c d e f g dp  低->高
// dp g f e d c b a
unsigned char smgNum[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00};

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

unsigned char keyScan() {
	// 用 低四位表示 每个按键是否被按下
	// 对应位 被按下, 就将对应位设置为1
	// 低四位中 高位->低位 分别对应S_KEY1 -> S_KEY4
	unsigned char keyState = 0x00;

	if (S_KEY1 == 0 || S_KEY2 == 0 || S_KEY3 == 0 || S_KEY4 == 0) {
		delayNMs(2); // 消抖, 确认是否被按下
		if (S_KEY1 == 0) {
			keyState |= S_KEY1_PUSHED;
		}

		if (S_KEY2 == 0) {
			keyState |= S_KEY2_PUSHED;
		}

		if (S_KEY3 == 0) {
			keyState |= S_KEY3_PUSHED;
		}

		if (S_KEY4 == 0) {
			keyState |= S_KEY4_PUSHED;
		}
	}

	// 返回keyState
	return keyState;
}

void keyLED() {
	LED1 = LED2 = LED3 = LED4 = 1;
	while (1) {
		unsigned char ret = keyScan();
		if (ret & S_KEY1_PUSHED) {
			LED1 = 0;
		}
		if (ret & S_KEY2_PUSHED) {
			LED2 = 0;
		}
		if (ret & S_KEY3_PUSHED) {
			LED3 = 0;
		}
		if (ret & S_KEY4_PUSHED) {
			LED4 = 0;
		}

		if (ret == NON_KEY_PUSH) {
			LED1 = LED2 = LED3 = LED4 = 1;
		}
	}
}

void keySMG() {
	SMG = smgNum[0];
	while (1) {
		unsigned char ret = keyScan();
		if (ret & S_KEY1_PUSHED) {
			smgFuncs[0]();
			SMG = smgNum[0];
			delayMs();
		}
		if (ret & S_KEY2_PUSHED) {
			smgFuncs[1]();
			SMG = smgNum[1];
			delayMs();
		}
		if (ret & S_KEY3_PUSHED) {
			smgFuncs[2]();
			SMG = smgNum[2];
			delayMs();
		}
		if (ret & S_KEY4_PUSHED) {
			smgFuncs[3]();
			SMG = smgNum[3];
			delayMs();
		}

		if (ret == NON_KEY_PUSH) {
			//delayMs();
			//SMG = smgNum[16];
		}
	}
}

void main() {
	keySMG();
}
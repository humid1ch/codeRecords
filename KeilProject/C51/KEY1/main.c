#include "reg52.h"
#include "intrins.h"

// 8位数码管引脚
#define SMG P0

sbit SMG_22 = P2 ^ 2;
sbit SMG_23 = P2 ^ 3;
sbit SMG_24 = P2 ^ 4;

#define KEY_PORT P1

#define KEY_UNPRESS 0
#define KEY1_PRESSED 1
#define KEY2_PRESSED 2
#define KEY3_PRESSED 3
#define KEY4_PRESSED 4
#define KEY5_PRESSED 5
#define KEY6_PRESSED 6
#define KEY7_PRESSED 7
#define KEY8_PRESSED 8
#define KEY9_PRESSED 9
#define KEY10_PRESSED 10
#define KEY11_PRESSED 11
#define KEY12_PRESSED 12
#define KEY13_PRESSED 13
#define KEY14_PRESSED 14
#define KEY15_PRESSED 15
#define KEY16_PRESSED 16

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

unsigned char keyColScanSingle() {
	unsigned char keyRes = KEY_UNPRESS;

	// 列 扫描, P13为左手第一列, P17为第一行
	// 即 P1的八位中, 低四位表示列, 高四位表示行
	// 目标列 拉低电平, 然后判断目标列是否有回到高电平

	// P2 = 0xFF;
	// delayNMs(1000);
	// P2 = 0x00;
	// delayNMs(1000);
	KEY_PORT = 0xF7;
	if (KEY_PORT != 0xF7) {
		delayNMs(10);
		if (KEY_PORT == 0x77) // 1行 1列
			keyRes = KEY1_PRESSED;
		if (KEY_PORT == 0xB7) // 2行 1列
			keyRes = KEY5_PRESSED;
		if (KEY_PORT == 0xD7) // 3行 1列
			keyRes = KEY9_PRESSED;
		if (KEY_PORT == 0xE7) // 4行 1列
			keyRes = KEY13_PRESSED;

		while (KEY_PORT != 0xF7)
			;
	}

	KEY_PORT = 0xFB;
	if (KEY_PORT != 0xFB) {
		delayNMs(10);
		if (KEY_PORT == 0x7B) // 1行 2列
			keyRes = KEY2_PRESSED;
		if (KEY_PORT == 0xBB) // 2行 2列
			keyRes = KEY6_PRESSED;
		if (KEY_PORT == 0xDB) // 3行 2列
			keyRes = KEY10_PRESSED;
		if (KEY_PORT == 0xEB) // 4行 2列
			keyRes = KEY14_PRESSED;

		while (KEY_PORT != 0xFB)
			;
	}

	KEY_PORT = 0xFD;
	if (KEY_PORT != 0xFD) {
		delayNMs(10);
		if (KEY_PORT == 0x7D) // 1行 3列
			keyRes = KEY3_PRESSED;
		if (KEY_PORT == 0xBD) // 2行 3列
			keyRes = KEY7_PRESSED;
		if (KEY_PORT == 0xDD) // 3行 3列
			keyRes = KEY11_PRESSED;
		if (KEY_PORT == 0xED) // 4行 3列
			keyRes = KEY15_PRESSED;

		while (KEY_PORT != 0xFD)
			;
	}

	KEY_PORT = 0xFE;
	if (KEY_PORT != 0xFE) {
		delayNMs(10);
		if (KEY_PORT == 0x7E) // 1行 4列
			keyRes = KEY4_PRESSED;
		if (KEY_PORT == 0xBE) // 2行 4列
			keyRes = KEY8_PRESSED;
		if (KEY_PORT == 0xDE) // 3行 4列
			keyRes = KEY12_PRESSED;
		if (KEY_PORT == 0xEE) // 4行 4列
			keyRes = KEY16_PRESSED;

		while (KEY_PORT != 0xFE)
			;
	}

	return keyRes;
}

unsigned char smgNum[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00};

void main() {
	unsigned char keyRes = KEY_UNPRESS;
	while (1) {
		// 选第一个数码管
		SMG_22 = 1;
		SMG_23 = 1;
		SMG_24 = 1;

		keyRes = keyColScanSingle();
		switch (keyRes) {
		case KEY1_PRESSED:
			SMG = smgNum[KEY1_PRESSED - 1];
			break;
		case KEY2_PRESSED:
			SMG = smgNum[KEY2_PRESSED - 1];
			break;
		case KEY3_PRESSED:
			SMG = smgNum[KEY3_PRESSED - 1];
			break;
		case KEY4_PRESSED:
			SMG = smgNum[KEY4_PRESSED - 1];
			break;
		case KEY5_PRESSED:
			SMG = smgNum[KEY5_PRESSED - 1];
			break;
		case KEY6_PRESSED:
			SMG = smgNum[KEY6_PRESSED - 1];
			break;
		case KEY7_PRESSED:
			SMG = smgNum[KEY7_PRESSED - 1];
			break;
		case KEY8_PRESSED:
			SMG = smgNum[KEY8_PRESSED - 1];
			break;
		case KEY9_PRESSED:
			SMG = smgNum[KEY9_PRESSED - 1];
			break;
		case KEY10_PRESSED:
			SMG = smgNum[KEY10_PRESSED - 1];
			break;
		case KEY11_PRESSED:
			SMG = smgNum[KEY11_PRESSED - 1];
			break;
		case KEY12_PRESSED:
			SMG = smgNum[KEY12_PRESSED - 1];
			break;
		case KEY13_PRESSED:
			SMG = smgNum[KEY13_PRESSED - 1];
			break;
		case KEY14_PRESSED:
			SMG = smgNum[KEY14_PRESSED - 1];
			break;
		case KEY15_PRESSED:
			SMG = smgNum[KEY15_PRESSED - 1];
			break;
		case KEY16_PRESSED:
			SMG = smgNum[KEY16_PRESSED - 1];
			break;

		default:
			break;
		}
	}
}
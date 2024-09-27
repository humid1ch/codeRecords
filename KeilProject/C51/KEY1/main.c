#include "reg52.h"
#include "intrins.h"

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

void delay10Us() {
	while (0)
		;
}

void delay10UsN(unsigned int n) {
	while (--n)
		delay10Us();
}

// LED 灯
#define LED P2

// 蜂鸣器
sbit BEEP = P2 ^ 5;

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

unsigned char keyMaps[][4] = {
	{KEY1_PRESSED, KEY2_PRESSED, KEY3_PRESSED, KEY4_PRESSED}, 
	{KEY5_PRESSED, KEY6_PRESSED, KEY7_PRESSED, KEY8_PRESSED}, 
	{KEY9_PRESSED, KEY10_PRESSED, KEY11_PRESSED, KEY12_PRESSED}, 
	{KEY13_PRESSED, KEY14_PRESSED, KEY15_PRESSED, KEY16_PRESSED}
};

unsigned char keyScan(); // 4x4矩阵按键 翻转扫描

typedef void (*smgSelFunc)(); // 数码管选择函数 指针
// 8个 1位数码管选择函数, 位选
void smg1();
void smg2();
void smg3();
void smg4();
void smg5();
void smg6();
void smg7();
void smg8();
// 数码管选择函数 数组
smgSelFunc smgFuncs[] = {smg1, smg2, smg3, smg4, smg5, smg6, smg7, smg8};

// 数码管显示值 数组, 十六进制 0~F
unsigned char smgNum[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00};

char defaultPasswd[8] = {1, 2, 3, 4, 5, 6, 7, 8};
char passwd[8] = {-1}; // 用于记录密码
char passwdIndex = -1; // 当前密码数组的有效尾元素下标
void smgPasswd();				  // 数码管显示密码函数
unsigned char confirmPWD();		  // 密码确认函数

// S9 S10 S11 S5 S6 S7 S1 S2 S3 S14, 对应1 2 3 4 5 6 7 8 9 0, 十个数字
// S4, 对应删除
// S8, 对应清零
// S16, 对应确认
void main() {
	unsigned char keyRes = KEY_UNPRESS;
	while (1) {
		smgPasswd(); // 数码管显示密码

		keyRes = keyScan(); // 按键扫描
		switch (keyRes) {
		case KEY1_PRESSED: // S1  7
			if (passwdIndex < 7)
				passwd[++passwdIndex] = 7;
			break;
		case KEY2_PRESSED: // S2  8
			if (passwdIndex < 7)
				passwd[++passwdIndex] = 8;
			break;
		case KEY3_PRESSED: // S3  9
			if (passwdIndex < 7)
				passwd[++passwdIndex] = 9;
			break;
		case KEY5_PRESSED: // S5  4
			if (passwdIndex < 7)
				passwd[++passwdIndex] = 4;
			break;
		case KEY6_PRESSED: // S6  5
			if (passwdIndex < 7)
				passwd[++passwdIndex] = 5;
			break;
		case KEY7_PRESSED: // S7  6
			if (passwdIndex < 7)
				passwd[++passwdIndex] = 6;
			break;
		case KEY9_PRESSED: // S9  1
			if (passwdIndex < 7)
				passwd[++passwdIndex] = 1;
			break;
		case KEY10_PRESSED: // S10  2
			if (passwdIndex < 7)
				passwd[++passwdIndex] = 2;
			break;
		case KEY11_PRESSED: // S11  3
			if (passwdIndex < 7)
				passwd[++passwdIndex] = 3;
			break;
		case KEY14_PRESSED: // S14  10
			if (passwdIndex < 7)
				passwd[++passwdIndex] = 0;
			break;
		case KEY4_PRESSED: // S4 回退
			if (passwdIndex >= 0)
				passwdIndex--;
			break;
		case KEY8_PRESSED: // S8 清空
			passwdIndex = -1;
			break;
		case KEY16_PRESSED: // S16 确认
			if (confirmPWD()) {
				// 验证成功
				// LED闪三下
				unsigned char i = 0;
				for (i = 0; i < 3; i++) {
					LED = 0x00;
					delayNMs(500);
					LED = 0xFF;
					delayNMs(500);
				}
			}
			else {
				// 验证失败
				// 蜂鸣器响半秒
				unsigned int i = 0;
				for (i = 0; i < 2000; i++) {
					BEEP = 0;
					delay10UsN(25);
					BEEP = 1;
					delay10UsN(25);
				}
			}
			passwdIndex = -1;	// 试图确认之后, 清空输入密码
			break;

		default:
			break;
		}
	}
}

unsigned char keyScan() {
	unsigned char keyRes = KEY_UNPRESS;

	unsigned char row = -1; // 行
	unsigned char col = -1; // 列
	// 列 扫描, P13为左手第一列, P17为第一行
	// 即 P1的八位中, 低四位表示列, 高四位表示行

	KEY_PORT = 0x0F;
	if (KEY_PORT != 0x0F) {
		delayNMs(10);
		if (KEY_PORT != 0x0F) {
			switch (KEY_PORT) {
			case 0x07:
				col = 0;
				break;
			case 0x0B:
				col = 1;
				break;
			case 0x0D:
				col = 2;
				break;
			case 0x0E:
				col = 3;
				break;

			default:
				break;
			}

			KEY_PORT = 0xF0;
			switch (KEY_PORT) {
			case 0x70:
				row = 0;
				break;
			case 0xB0:
				row = 1;
				break;
			case 0xD0:
				row = 2;
				break;
			case 0xE0:
				row = 3;
				break;

			default:
				break;
			}

			while (KEY_PORT != 0xF0)
				;
		}

		keyRes = keyMaps[row][col];
	}

	return keyRes;
}

// 使用 passwd 数组, 在数码管显示密码
// passwdIndex 之前的不做显示
void smgPasswd() {
	char i = 0;
	for (i = 0; i <= passwdIndex; i++) {
		smgFuncs[i]();			 // 选择第i个数码管, 第0~7个数码管
		SMG = smgNum[passwd[i]]; // 显示 passwd[i]的数值, 位置 与passwd[i]对应
		delayNMs(2);
	}
	SMG = smgNum[16];
}

// 确认密码
unsigned char confirmPWD() {
	unsigned char i = 0;
	if (passwdIndex != 7)	// 输入不到8位
		return 0;

	for (i = 0; i < 8; i++) {
		if (passwd[i] != defaultPasswd[i])	// 输入 与 正确密码不匹配
			return 0;
	}

	return 1;
}

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
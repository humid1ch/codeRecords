#include <reg52.h>
#include <intrins.h>

typedef unsigned int u16;
typedef unsigned char u8;
typedef unsigned long u32;

// LED 点阵 列引脚, 二极管阴极
#define LED_DOT_COL P0

// 数据输入引脚, 1bit
sbit SER = P3 ^ 4;
// 数据并行输出信号引脚
sbit rCLK = P3 ^ 5;
// 数据移位存储引脚
sbit SRCLK = P3 ^ 6;

void delay10Us() {
	while (0)
		;
}

void delayMs() {
	unsigned char data i, j;

	_nop_();
	i = 2;
	j = 199;
	do {
		while (--j)
			;
	} while (--i);
}

void delayN10Us(unsigned int n) {
	while (--n)
		delay10Us();
}

void delayNMs(u16 n) {
	while (--n)
		delayMs();
}

#define SRCLK_UP_ONCE() \
	{                   \
		SRCLK = 0;      \
		delay10Us();    \
		SRCLK = 1;      \
		delay10Us();    \
		SRCLK = 0;      \
	}

#define rCLK_UP_ONCE() \
	{                  \
		rCLK = 0;      \
		delay10Us();   \
		rCLK = 1;      \
		delay10Us();   \
		rCLK = 0;      \
	}

// 参数 是需要经过 595 并行发送的数据
void HC595SendByte(u8 u8data) {
	u8 i = 0;
	// 将u8data从高位到低位 依次写入595的移位寄存器
	for (i = 0; i < 8; i++) {
		SER = u8data >> 7; // u8data最高位赋给SER引脚
		u8data <<= 1;	   // 更新u8data的最高位

		// 控制SRCLK引脚, 将赋给SER的数据写入到移位寄存器中
		SRCLK_UP_ONCE();
	}

	// 控制 rCLK引脚, 将移位寄存器数据拷贝到数据寄存器, 并 并行输出
	rCLK_UP_ONCE();
}
/*
595输出的数据 是控制LED点阵 每行的电平的
595中 数据寄存器 从高到低位 对应的引脚是			而 LED点阵中, 从第1->8行 对应引脚为
1-DPh										  1-DPa
2-DPg										  2-DPb
3-DPf										  3-DPc
4-DPe										  4-DPd
5-DPd										  5-DPe
6-DPc										  6-DPf
7-DPb										  7-DPg
8-DPa										  8-DPh

所以, 若需要实现 以 输入数据从高位到低位 控制 LED点阵的第1 到 第8行
就需要, 先向595的移位寄存器中写入数据的高位数据
*/

// 4个595串起来, 可以并行发送4字节数据
// 通过拉高RCLK引脚, 可以将移位寄存器的数据, 通过SER引脚, 移位到后面的595中
// 所以, 每拉高8次RCLK引脚, 就会有8bit数据被挤到后面的595中
// 直到拉高32次RCLK引脚, 就可以将32位数据, 按顺序分配到4个595中
// 然后 通过SRCLK将4个595中 移位寄存器的数据一起并行发送出去
void HC595x4SendByte(u16 u16Rows, u16 u16Cols) {
	u8 i = 0;
	// 将u16Rows从高位到低位 依次写入595的移位寄存器
	for (i = 0; i < 16; i++) {
		SER = u16Rows >> 15; // u16Rows最高位赋给SER引脚
		u16Rows <<= 1;		 // 更新u16Rows的最高位

		// 控制SRCLK引脚, 将赋给SER的数据写入到移位寄存器中
		SRCLK_UP_ONCE();
	}

	// 将u16Cols从高位到低位 依次写入595的移位寄存器
	for (i = 0; i < 16; i++) {
		SER = u16Cols >> 15; // u16Cols最高位赋给SER引脚
		u16Cols <<= 1;		 // 更新u16Cols的最高位

		// 控制SRCLK引脚, 将赋给SER的数据写入到移位寄存器中
		SRCLK_UP_ONCE();
	}

	// 控制 rCLK引脚, 将4个595的移位寄存器数据拷贝到数据寄存器, 并 并行输出
	rCLK_UP_ONCE();
}

// 1 2 3 4 5 6 7 8 col
u8 gU8LedCols[8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};
// 1 2 3 4 5 6 7 8 row
u8 gU8LedRows[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

void ledRowCle() {
	char i = 0;
	LED_DOT_COL = 0xFF;
	LED_DOT_COL = 0x00;
	for (i = 0; i < 8; i++) {
		HC595SendByte(gU8LedRows[i]);
		delayNMs(100);
	}
	for (i = 7; i >= 0; i--) {
		HC595SendByte(gU8LedRows[i]);
		delayNMs(100);
	}
}

void ledColCle() {
	char i = 0;
	LED_DOT_COL = 0xFF;
	HC595SendByte(0xFF);
	for (i = 0; i < 8; i++) {
		LED_DOT_COL = gU8LedCols[i];
		delayNMs(100);
	}
	for (i = 7; i >= 0; i--) {
		LED_DOT_COL = gU8LedCols[i];
		delayNMs(100);
	}
}

void ledDiagonal() {
	char i = 0;
	char j = 7;
	LED_DOT_COL = 0xFF;
	HC595SendByte(0x00);
	for (i = 0, j = 7; i < 8, j >= 0; i++, j--) {
		LED_DOT_COL = gU8LedCols[i];
		HC595SendByte(gU8LedRows[i]);
		delayN10Us(30);
		HC595SendByte(gU8LedRows[j]);
		delayN10Us(30);
		LED_DOT_COL = 0xFF;
		HC595SendByte(0x00);
		delayMs();
	}
}

void ledDisplay(u8* u8Rows) {
	char i = 0;
	for (i = 0; i < 8; i++) {
		HC595SendByte(0x00);		 //消影
		HC595SendByte(u8Rows[i]);	 //LED行数据
		LED_DOT_COL = gU8LedCols[i]; //LED列数据
		delayNMs(3);				 //保持时间
	}
}

// 为能够 无缝循环显示, 需要将屏幕首帧需要显示的画面, 在结尾放一下
// 即 数组中首8个元素, 需要在数组尾再放一遍
void ledLeftScroll(const u8* array, const u8 size) {
	u8 i = 0;
	u8 ledOffset = 0; // led显示数数组的偏移量
	while (ledOffset < size - 7) {
		for (i = 0; i < 5; i++) {
			ledDisplay(array + ledOffset);
		}
		ledOffset++;
	}
}

void main() {
	// u8 u8SmallRows[] = {0x00, 0x30, 0x78, 0x3C, 0x3C, 0x78, 0x30, 0x00};
	// u8 u8HelloBig[] = {0x00, 0x7E, 0x7E, 0x18, 0x18, 0x7E, 0x7E, 0x00, 0x00, 0x7E, 0x7E, 0x5A, 0x5A, 0x5A, 0x5A, 0x00, 0x00, 0x7E, 0x7E, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x7E, 0x7E, 0x06, 0x06, 0x06, 0x06, 0x00,
	// 				   0x00, 0x7E, 0x7E, 0x42, 0x42, 0x7E, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00};
	u8 u8HelloSmall[] = {0x01, 0x7D, 0x11, 0x11, 0x7D, 0x01, 0x01, 0x7D, 0x55, 0x55, 0x55, 0x01, 0x01, 0x7D, 0x05, 0x05,
						 0x05, 0x01, 0x01, 0x7D, 0x05, 0x05, 0x05, 0x01, 0x01, 0x7D, 0x45, 0x45, 0x7D, 0x01, 0x01, 0x7D, 0x11, 0x11, 0x7D, 0x01, 0x01};

	while (1) {
		ledLeftScroll(u8HelloSmall, sizeof(u8HelloSmall) / sizeof(u8HelloSmall[0]));
	}
}
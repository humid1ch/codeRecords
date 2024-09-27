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
void delay10UsN(unsigned char data n) {
	while (--n)
		delay10Us();
}

// 蜂鸣器
#define FEQUENCY_S 2000 // 蜂鸣器工作频率
sbit BEEP = P2 ^ 5;		// 蜂鸣器引脚
// 设置蜂鸣器 500us为一次脉冲, 占空比50%, 即 高250us, 低250us
// 约 2000次脉冲/s
void beepSet500() {
	BEEP = 0;
	delay10UsN(25);
	BEEP = 1;
	delay10UsN(25);
}

// 独立按键
// 宏定义 用于独立按键 按压的记录
#define NON_KEY_PRESS 0x00
#define S_KEY1_PRESSED 0x08
#define S_KEY2_PRESSED 0x04

// 四个按键, 连接引脚默认为高电平, 按下按键 与地导通, 变为低电平
sbit S_KEY1 = P3 ^ 1;
sbit S_KEY2 = P3 ^ 0;

// 中断 不使用
// sbit S_KEY3 = P3 ^ 2;
// sbit S_KEY4 = P3 ^ 3;
#define INT0_ON  \
	{            \
		EA = 1;  \
		EX0 = 1; \
		IT0 = 1; \
	}

#define INT1_ON  \
	{            \
		EA = 1;  \
		EX1 = 1; \
		IT1 = 1; \
	}

// 直流电机
sbit MOTOR_IN = P1 ^ 0; // 直流电机负极引脚
// 给高电平, 通过U14模块转换为低电平 激活直流电机
#define MOTOR_ON MOTOR_IN = 1
#define MOTOR_OFF MOTOR_IN = 0

typedef void (*smgSelFunc)();
// 8位数码管引脚
#define SMG P0
// 3->8译码器的三个引脚
// 板子上 右边数码管的是较小值
sbit SMG_22 = P2 ^ 2;
sbit SMG_23 = P2 ^ 3;
sbit SMG_24 = P2 ^ 4;
// 1位数码管选择函数
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

// 数码管选择函数 数组
smgSelFunc smgFuncs[] = {smg1, smg2, smg5, smg6};

// 板子使用 共阴数码管, 各引脚给高电平点亮边
// a b c d e f g dp  低->高
// dp g f e d c b a
unsigned char smgNum[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00};

// 独立按键扫描 函数
// 本案例中, 不需要处理同时按下多个按键的情况
unsigned char keyScanNoDown() { // KEY3 KEY4使用中断进行实现
	unsigned char keys = NON_KEY_PRESS;
	if (S_KEY1 == 0 || S_KEY2 == 0 /* || S_KEY3 == 0 */) {
		delayNMs(10); // 消抖, 用于确认是否被按下

		if (S_KEY1 == 0)
			keys = S_KEY1_PRESSED;
		else if (S_KEY2 == 0)
			keys = S_KEY2_PRESSED;
		// else if (S_KEY3 == 0)
		// 	keys = S_KEY3_PRESSED;
	}

	while ((S_KEY1 == 0) || (S_KEY2 == 0) /* || (S_KEY3 == 0) */)
		; //等待按键弹起

	return keys;
}

// 中断, 不适用
// unsigned char keyScanDown() {
// 	unsigned char keys = NON_KEY_PRESS;

// 	if (S_KEY4 == 0) {
// 		delayNMs(10);
// 		if (S_KEY4 == 0)
// 			keys = S_KEY4_PRESSED;
// 	}
//     while(S_KEY4 == 0); //等待按键弹起

// 	return keys;
// }

// 数码管显示
void showDown(unsigned char defautltCntDown, unsigned char cntDownChange, unsigned char cntDown) {
	//预设十位
	smgFuncs[0]();
	SMG = smgNum[(defautltCntDown + cntDownChange) / 10];
	delayNMs(5);
	// 预设个位
	smgFuncs[1]();
	SMG = smgNum[(defautltCntDown + cntDownChange) % 10];
	delayNMs(4);

	// 计时十位
	smgFuncs[2]();
	SMG = smgNum[(cntDown / 10)];
	delayNMs(5);
	// 计时个位
	smgFuncs[3]();
	SMG = smgNum[(cntDown % 10)];
	delayNMs(4);
	SMG = smgNum[16];
}

unsigned char defautltCntDown = 10; // 倒计时 初始值为10, 单位秒
unsigned char cntDownChange = 0;	// 倒计时 预设修改
unsigned char cntDown;				// 倒计时预设值
unsigned char key4Press = 0;		// 按键4是否被按下过
unsigned char key3Press = 0;		// 按键4是否被按下过

// 外部中断0, 中断号0
void key3() interrupt 0 {
	key3Press = 1;
}
// 外部中断1, 中断号2
void key4() interrupt 2 {
	key4Press = 1;
}

void timer0() interrupt 1 {

}

void main() {
	INT0_ON; // 外部中断0, 开启		P32
	INT1_ON; // 外部中断1, 开启		P33
	MOTOR_OFF;
	while (1) {
		unsigned char keyRes = keyScanNoDown();
		unsigned char isDowning = 0; // 是否正在倒计时
		unsigned int beepCnt = 2000;
		// 计算倒计时预设值
		cntDown = defautltCntDown + cntDownChange;

		showDown(defautltCntDown, cntDownChange, cntDown);

		if (keyRes == S_KEY1_PRESSED) { // 倒计时 预设值++
			if (cntDownChange < 30)
				cntDownChange++;
		}
		if (keyRes == S_KEY2_PRESSED) {
			if (cntDownChange > 0)
				cntDownChange--;
		}

		 // 中断, 不适用
		if (key3Press) {
			isDowning = 1;

			// 直流电机开转
			MOTOR_ON;

			// 正在倒计时 或 key4被按压过  进行循环
			while (1) {
				unsigned int i = 0;
				// 扫描4是否被按下
				while (key4Press) {
					if (isDowning == 1) {
						isDowning = 0;
						MOTOR_OFF;
					}
					else {
						isDowning = 1;
						MOTOR_ON;
					}
					key4Press = 0;
				}
				showDown(defautltCntDown, cntDownChange, cntDown);

				if (isDowning && cntDown > 0)
					cntDown--;
				else if (cntDown == 0) {
					break;
				}

				for (i = 0; i < 80; i++) {
					while (key4Press) {
						if (isDowning == 1) {
							isDowning = 0;
							MOTOR_OFF;
						}
						else {
							isDowning = 1;
							MOTOR_ON;
						}
						key4Press = 0;
					}
					showDown(defautltCntDown, cntDownChange, cntDown);
				}
			}
			smgFuncs[0]();
			SMG = smgNum[16];

			// 直流电机停转
			MOTOR_OFF;
			// 蜂鸣器响1s
			for (beepCnt; beepCnt > 0; beepCnt--)
				beepSet500();
			
			key3Press = 0;
		}
	}
}
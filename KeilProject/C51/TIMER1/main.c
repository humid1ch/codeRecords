#include <reg52.h>

sbit led1 = P2 ^ 0;
sbit led8 = P2 ^ 7;

// 定时器0, TMOD低四位, 时钟11.0592MHz 计时1毫秒
void timer0Init();
// 定时器1, TMOD高四位, 时钟11.0592MHz 计时1毫秒
void timer1Init();

unsigned int timer0Cnt = 0;
unsigned int timer1Cnt = 0;

void main() {
	timer0Init();
	timer1Init();

	while (1)
		;
}

// 定时器0, TMOD低四位, 时钟11.0592MHz 计时10毫秒
void timer0Init() {
	EA = 1;	 // 先打开中断总开关
	ET0 = 1; // 打开定时器0的开关

	TMOD &= 0xF0; // TMOD低四位清零
	TMOD |= 0x01; // 无外部中断, 定时模式, 方式1: 16位定时器工作
	TH0 = 0xDC;	  // TH0寄存器初始值, 计数高八位
	TL0 = 0x00;	  // TL0寄存器初始值, 计数低八位
	TF0 = 0;	  // TF0, 溢出标志位, 置0
	TR0 = 1;	  // 运行定时器0
}

// 定时器1, TMOD高四位, 时钟11.0592MHz 计时10毫秒
void timer1Init() {
	EA = 1;	 // 先打开中断总开关
	ET1 = 1; // 打开定时器0的开关

	TMOD &= 0x0F; // TMOD高四位清零
	TMOD |= 0x10; // 无外部中断, 定时模式, 方式1: 16位定时器工作
	TH1 = 0xDC;	  // TH1寄存器初始值, 计数高八位
	TL1 = 0x00;	  // TL1寄存器初始值, 计数第八位
	TF1 = 0;	  // TF1, 定时器1溢出标志位, 置0
	TR1 = 1;	  // 运行定时器1
}

void timer0Func() interrupt 1 {
	timer0Cnt++;
	TH0 = 0xDC; // TH0寄存器初始值, 计数高八位
	TL0 = 0x00; // TL0寄存器初始值, 计数低八位
    
	if (timer0Cnt >= 50) {

		timer0Cnt = 0;
		led1 = ~led1;
	}
}

void timer1Func() interrupt 3 {
	timer1Cnt++;
	TH1 = 0xDC; // TH1寄存器初始值, 计数高八位
	TL1 = 0x00; // TL1寄存器初始值, 计数第八位

	if (timer1Cnt >= 100) {

		timer1Cnt = 0;
		led8 = ~led8;
	}
}
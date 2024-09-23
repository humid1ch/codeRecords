#include <reg52.h>
#include "delay50Us.h"
#include "delay10Us.h"

#define FEQUENCY_S 2000 // 每秒两千次

///////////////////中音部
#define c 1200
#define cp 1130
#define d 1060
#define dp 1000
#define e 950
#define f 900
#define fp 840
#define g 800
#define gp 750
#define a 710
#define ap 670
#define b 630
///////////////////高音部
#define C 600
#define Cp 560
#define D 530
#define Dp 500
#define E 470
#define F 450
#define Fp 420
#define G 400
#define Gp 380
#define A 360
#define Ap 340
#define B 320
///////////////////低音部
#define cc 2390
#define ccp 2260
#define dd 2130
#define ddp 2010
#define ee 1890
#define ff 1790
#define ffp 1690
#define gg 1590
#define ggp 1510
#define aa 1420
#define aap 1340
#define bb 1270

#define xx 0

typedef struct MusicNote {
	unsigned int freq;	  // 音符频率
	unsigned char second; // 音符持续时间
} MNote;

sbit BEEP = P2 ^ 5;

MNote music[] = {{C, 1}, {b, 1}, {C, 1}, {E, 1}, {b, 1}, {a, 1}, {g, 1}, {a, 1}, {C, 1}, {g, 1}, {f, 1}, {e, 1}, {f, 1}, {C, 1}, {b, 1}, {g, 1}, {a, 1}, {b, 1}, {C, 1}, {E, 1}, {D, 1}, {C, 1}, {b, 1}, {C, 1}, {E, 1}/*, {b, 1}, {g, 1}, {a, 1}, {b, 1}, {C, 1}, {D, 1}, {E, 1}, {E, 1}, {F, 1}, {E, 1}, {D, 1}, {C, 1}, {b, 1}, {E, 1}, {gp, 1}, {b, 1}, {a, 1}, {a, 1}, {b, 1}, {C, 1}, {D, 1}, {E, 1}, {D, 1}, {G, 1}, {F, 1}, {E, 1}, {D, 1}, {C, 1}, {b, 1}, {C, 1}, {a, 1}, {a, 1}, {a, 1}, {g, 1}, {f, 1}, {g, 1}, {a, 1}, {C, 1}, {b, 1}, {b, 1}, {C, 1}, {D, 1}, {b, 1}, {b, 1}, {xx, 1}, {a, 1}, {a, 1}, {a, 1}, {b, 1}, {C, 1}, {D, 1}, {E, 1}, {D, 1}, {G, 1}, {F, 1}, {E, 1}, {D, 1}, {C, 1}, {b, 1}, {C, 1}, {a, 1}, {a, 1}, {a, 1}, {g, 1}, {f, 1}, {g, 1}, {a, 1}, {C, 1}, {b, 1}, {b, 1}, {C, 1}, {D, 1}, {F, 1}, {E, 1}, {E, 1}, {D, 1}, {C, 1}, {b, 1}, {gp, 1}, {C, 1}, {b, 1}, {C, 1}, {E, 1}, {b, 1}, {a, 1}, {g, 1}, {a, 1}, {C, 1}, {g, 1}, {f, 1}, {e, 1}, {f, 1}, {C, 1}, {b, 1}, {g, 1}, {a, 1}, {b, 1}, {C, 1}, {E, 1}, {D, 1}, {C, 1}, {b, 1}, {C, 1}, {E, 1}, {b, 1}, {g, 1}, {a, 1}, {b, 1}, {C, 1}, {D, 1}, {E, 1}, {E, 1}, {F, 1}, {E, 1}, {D, 1}, {C, 1}, {b, 1}, {E, 1}, {gp, 1}, {b, 1}, {a, 1}*/};

//unsigned int frequencys[] = {100, 125, 160, 200, 250, 320, 400, 500, 625, 800, 1000, 1250, 1600, 2000, 2500, 3125, 4000, 5000};

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

void beepMusic(const MNote* mNote, unsigned char duty) {
	unsigned int cycle = 1000000 / mNote->freq;			 // freq频率下的 周期
	float dutyF = (float)duty / 100.0;					 // 占空比 小数
	unsigned int upDuty = cycle * dutyF;				 // 高电平延迟 比
	unsigned int downDuty = cycle * (1.0 - dutyF);		 // 低电平延迟 比
	unsigned int cnt = mNote->freq * mNote->second / 10; // second秒下的循环次数

	while (--cnt) {
		BEEP = 0;
		delay50UsN(downDuty / 50);
		BEEP = 1;
		delay50UsN(upDuty / 50);
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
	// unsigned char second = 1;				// 需要脉冲多少秒?
	// unsigned int cnt = FEQUENCY_S * second; // 脉冲一定秒数 需要脉冲的次数

	// while (--cnt)
	// 	beepSet500();

	// char i = sizeof(frequencys) / sizeof(frequencys[0]) - 1;
	// for (i; i >= 0; i--) {
	// 	beep(frequencys[i], 0.2, 1);
	// }

	char i = sizeof(music) / sizeof(music[0]);
	char j;
	for (j = 0; j < i; j++) {
		beepMusic(music + j, 50);
	}

	// BEEP脉冲结束之后, 如果程序结束, 程序会再次执行, BEEP脉冲会继续, BEEP会一直响
	// 所以 禁止程序结束
	while (1)
		;
}
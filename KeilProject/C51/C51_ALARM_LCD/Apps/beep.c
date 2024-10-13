#include "reg52.h"
#include "beep.h"

sbit BEEP = P2 ^ 5;

static void _BEEP_Pulse500();

void BEEP_Init() {}

// 蜂鸣器响多少秒
void BEEP_On(u8 u8seconds) {
	u16 cnt = FEQUENCY_S * u8seconds / 1.8; // 脉冲一定秒数 需要脉冲的次数

    while(cnt--)
        _BEEP_Pulse500();
}

// 设置蜂鸣器 500us为一次脉冲, 占空比50%, 即 高250us, 低250us
// 约 2000次脉冲/s
static void _BEEP_Pulse500() {
	BEEP = 0;
	delayN10Us(25);
	BEEP = 1;
	delayN10Us(25);
}


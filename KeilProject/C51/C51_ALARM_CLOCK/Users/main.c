#include "reg52.h"
#include "util.h"
#include "ds1302.h"
#include "smg.h"
#include "skey.h"

extern u8 gDS1302_Time[7];

u8 alarmSecondCnt = 0; // 用于闹钟秒计时, 最大60
u8 alarmMiniteCnt = 0; // 用于闹钟分钟计时, 最大60
u8 alarmHourCnt = 0;   // 用于闹钟小时计时, 最大23

u8 setHour = 0;   // 用于小时设置, 最大23
u8 setMinite = 0; // 用于分钟设置, 最大59
u8 setSecond = 0; // 用于秒设置, 最大59
u8 setYear = 0;   // 用于年设置, 初始为20
u8 setMonth = 0;  // 用于月设置, 最大为12
u8 setDay = 0;    // 用于日设置, 最大为31
u8 setWeek = 0;   // 用于周几设置, 最大为7

void Time_Set();
void Time_SMG_Display();

void main() {
    Time_Set(); // 启动之后, 先手动设置时间
    DS1302_Init();
    SMG_Init();
    SKEY_Init();

    while (1) {
        

        DS1302_GetTime();
        Time_SMG_Display();
    }
}

void Time_Set() {
    u8 smgCode[4] = { 0 };
    
}

void Time_SMG_Display() {
    u8 i = 0, j = 0;

    for (i = 0, j = 0; i < 3; i++) {
        smgFuncs[j]();
        j++;
        SMG = smgNum[gDS1302_Time[2 - i] / 16]; // 十位
        delayNMs(2);
        SMG = 0x00;

        smgFuncs[j]();
        j++;
        SMG = smgNum[gDS1302_Time[2 - i] % 16]; // 个位
        delayNMs(2);
        SMG = 0x00;

        if (j < 7) {
            smgFuncs[j]();
            j++;
            SMG = 0x40; // -
            delayNMs(2);
            SMG = 0x00;
        }
    }
}
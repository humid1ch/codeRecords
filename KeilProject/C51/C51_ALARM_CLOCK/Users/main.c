/*
K1: 切换显示内容, 时间, 日期, 温度
K3: 重新设置时间
K4: 设置闹钟

设置闹钟或时间时
K1 确认
K2 +
K3 -
*/
#include "reg52.h"
#include "util.h"
#include "ds1302.h"
#include "ds18b20.h"
#include "beep.h"
#include "smg.h"
#include "skey.h"

extern smgSelFunc smgFuncs[];

extern u8 gDS1302_Time[7];

u8 alarmSecond = 0; // 用于闹钟秒计时, 最大60
u8 alarmMinite = 0; // 用于闹钟分钟计时, 最大60
u8 alarmHour = 0;   // 用于闹钟小时计时, 最大23
u8 alarmSet = 0;

u8 setHour = 0;   // 用于小时设置, 最大23
u8 setMinite = 0; // 用于分钟设置, 最大59
u8 setSecond = 0; // 用于秒设置, 最大59
u8 setYear = 20;  // 用于年设置, 初始为20
u8 setMonth = 1;  // 用于月设置, 最大为12
u8 setDay = 1;    // 用于日设置, 最大为31

void Alarm_Beep();
void Alarm_Set();
void Date_Set();
void Time_SMG_Display();
void Date_SMG_Display();
void Temp_Calculate(u16* u16data, float* temperature, u8* negFlag);
void Temp_SMG_Display(float temp, u8 neg);

void main() {
    u16 u16tempData = 0; // 接收 ds18b20 获取的值
    u8 negFlag = 0;
    float temperature = 0.0;
    u8 keyRes = 0;
    u8 displayFlag = 0; // 0 1 2, 0显示时间, 1显示日期, 2显示温度

    BEEP_Init();    // 蜂鸣器初始化
    SMG_Init();     //
    SKEY_Init();    // 独立按键初始化
    DS1302_Init();  //
    DS18B20_Init(); // DS18B20 温度传感器初始化

    while (1) {
        keyRes = SKEY_Scan();
        switch (keyRes) {
        case SKEY1_PRESSED: // 显示切换
            displayFlag++;
            break;

        case SKEY3_PRESSED: // 日期设置, 日期不带点, 时间带点
            Date_Set();     // 日期设置函数
            DS1302_Init();  // 再次初始化
            break;

        case SKEY4_PRESSED: // 闹钟设置
            Alarm_Set();    // 闹钟设置
            break;
        }

        DS1302_GetTime(); // 获取时间

        DS18B20_ReadTemperature(&u16tempData);                // 读取 16位温度量
        Temp_Calculate(&u16tempData, &temperature, &negFlag); // 计算具体温度

        displayFlag %= 3;

        switch (displayFlag) {
        case 0:
            Time_SMG_Display();
            break;

        case 1:
            Date_SMG_Display();
            break;

        case 2:
            Temp_SMG_Display(temperature, negFlag);
            negFlag = 0;
            break;
        }
        Alarm_Beep(); // 判断闹钟是否到时
    }
}

// 温度显示函数
void Temp_SMG_Display(float temp, u8 neg) {
    u16 tempInt = (u16)(temp *= 10);
    u8 mod = 0;
    u8 smgCode[5] = {0};
    char cnt = 3;

    if (neg)
        smgCode[0] = 18;
    else
        smgCode[0] = 17;

    while (tempInt) {
        mod = tempInt % 10;
        tempInt /= 10;
        smgCode[cnt--] = mod;
    }
    // C
    smgCode[4] = 12;

    SMG_Display(smgCode, smgCode + 4);

    // 点
    smgFuncs[2]();
    SMG = smgNum[16];
    delayMs();
    SMG = 0x00;
}

// 日期显示函数, 年月日
void Date_SMG_Display() {
    u8 smgCode[8] = {0};

    // 年
    smgCode[0] = gDS1302_Time[6] / 16;
    smgCode[1] = gDS1302_Time[6] % 16;
    smgCode[2] = 18;
    // 月
    smgCode[3] = gDS1302_Time[4] / 16;
    smgCode[4] = gDS1302_Time[4] % 16;
    smgCode[5] = 18;
    // 日
    smgCode[6] = gDS1302_Time[3] / 16;
    smgCode[7] = gDS1302_Time[3] % 16;

    SMG_Display(smgCode, smgCode + 7);
}

// 时间显示函数
void Time_SMG_Display() {
    u8 smgCode[8] = {0};

    // 时
    smgCode[0] = gDS1302_Time[2] / 16;
    smgCode[1] = gDS1302_Time[2] % 16;
    smgCode[2] = 18;
    // 分
    smgCode[3] = gDS1302_Time[1] / 16;
    smgCode[4] = gDS1302_Time[1] % 16;
    smgCode[5] = 18;
    // 秒
    smgCode[6] = gDS1302_Time[0] / 16;
    smgCode[7] = gDS1302_Time[0] % 16;

    SMG_Display(smgCode, smgCode + 7);
}

// 闹钟判断
void Alarm_Beep() {
    if (alarmSet && gDS1302_Time[0] == alarmSecond && gDS1302_Time[1] == alarmMinite && gDS1302_Time[2] == alarmHour) {
        alarmSet = 0;
        BEEP_On(2);
    }
}

// 本闹钟思路, 不采用定时器, 而是 与DS1302读取数据做对比
// 采用定时器也可以, 可以将定时的时间均转换为秒, 设定定时器时间
// 数码管 位数有限, 所以 以闪烁作为提示, 表示当前修改的数据为: 时 或 分 或 秒
void Alarm_Set() {
    u8 keyRes = 0;         // 记录按键扫描返回值
    u8 confirmFlag = 0;    // 记录确认
    u8 alarmSecondCnt = 0; // 用于闹钟秒计时, 最大59
    u8 alarmMiniteCnt = 0; // 用于闹钟分钟计时, 最大59
    u8 alarmHourCnt = 0;   // 用于闹钟小时计时, 最大23
    u8 smgCode[8] = {0};
    char j = 0;

    u8 i = 0;
    // i: 0秒 1分 2时
    for (i = 0; i < 3; i++) {
        while (!confirmFlag) {
            smgCode[0] = alarmHourCnt / 10;
            smgCode[1] = alarmHourCnt % 10;
            smgCode[2] = 18;
            smgCode[3] = alarmMiniteCnt / 10;
            smgCode[4] = alarmMiniteCnt % 10;
            smgCode[5] = 18;
            smgCode[6] = alarmSecondCnt / 10;
            smgCode[7] = alarmSecondCnt % 10;

            if (i == 0) {
                // 秒
                j = 7;
                while (j--)
                    SMG_Display(smgCode, smgCode + 7);
                // 秒闪烁
                smgCode[6] = 17;
                smgCode[7] = 17;
                j = 7;
                while (j--)
                    SMG_Display(smgCode, smgCode + 7);
            }
            else if (i == 1) {
                // 分
                j = 7;
                while (j--)
                    SMG_Display(smgCode, smgCode + 7);
                // 分闪烁
                smgCode[3] = 17;
                smgCode[4] = 17;
                j = 7;
                while (j--)
                    SMG_Display(smgCode, smgCode + 7);
            }
            else {
                // 时
                j = 7;
                while (j--)
                    SMG_Display(smgCode, smgCode + 7);
                // 时闪烁
                smgCode[0] = 17;
                smgCode[1] = 17;
                j = 7;
                while (j--)
                    SMG_Display(smgCode, smgCode + 7);
            }

            keyRes = SKEY_Scan();
            switch (keyRes) {
            case SKEY1_PRESSED:
                confirmFlag = 1;
                break;

            case SKEY2_PRESSED:
                if (i == 0) {
                    if (alarmSecondCnt < 59)
                        alarmSecondCnt++;
                }
                else if (i == 1) {
                    if (alarmMiniteCnt < 59)
                        alarmMiniteCnt++;
                }
                else {
                    if (alarmHourCnt < 59)
                        alarmHourCnt++;
                }
                break;

            case SKEY3_PRESSED:
                if (i == 0) {
                    if (alarmSecondCnt > 0)
                        alarmSecondCnt--;
                }
                else if (i == 1) {
                    if (alarmMiniteCnt > 0)
                        alarmMiniteCnt--;
                }
                else {
                    if (alarmHourCnt > 0)
                        alarmHourCnt--;
                }
                break;
            }
        }
        confirmFlag = 0;
    }

    // 已设置闹钟
    alarmSet = 1;

    // 提示闹钟设置完成
    DS1302_GetTime(); // 获取当前时间
    alarmHour = (gDS1302_Time[2] >> 4) * 10 + (gDS1302_Time[2] & 0x0F);
    alarmMinite = (gDS1302_Time[1] >> 4) * 10 + (gDS1302_Time[1] & 0x0F);
    alarmSecond = (gDS1302_Time[0] >> 4) * 10 + (gDS1302_Time[0] & 0x0F);

    // 计算闹钟结束时间
    alarmHour += alarmHourCnt;
    alarmMinite += alarmMiniteCnt;
    alarmSecond += alarmSecondCnt;

    // 进位 修正闹钟结束时间
    alarmMinite += alarmSecond / 60;
    alarmSecond %= 60;
    alarmHour += alarmMinite / 60;
    alarmMinite %= 60;
    alarmHour %= 24;

    // 十进制转BCD, 方便后面与 ds1302获取时间做对比
    alarmMinite = ((alarmMinite / 10) << 4) + (alarmMinite % 10);
    alarmHour = ((alarmHour / 10) << 4) + (alarmHour % 10);
    alarmSecond = ((alarmSecond / 10) << 4) + (alarmSecond % 10);
}

void Date_Set() {
    u8 keyRes = 0; // 记录按键扫描返回值, 用于记录数据位置
    u8 smgCode[8] = {0};
    u8 j = 0;
    u8 i = 0;
    // i: 0(年) 1(月) 2(日) 3(时) 4(分) 5(秒)
    // 时分秒要带 点, 便于区分 是 时分秒 还是 年月日
    while (i < 6) {
        if (i < 3) {
            smgCode[0] = setYear / 10;
            smgCode[1] = setYear % 10;
            smgCode[2] = 18;
            smgCode[3] = setMonth / 10;
            smgCode[4] = setMonth % 10;
            smgCode[5] = 18;
            smgCode[6] = setDay / 10;
            smgCode[7] = setDay % 10;

            if (i == 0) {
                // 年
                j = 7;
                while (j--)
                    SMG_Display(smgCode, smgCode + 7);
                // 年闪烁
                smgCode[0] = 17;
                smgCode[1] = 17;
                j = 7;
                while (j--)
                    SMG_Display(smgCode, smgCode + 7);
            }
            else if (i == 1) {
                // 月
                j = 7;
                while (j--)
                    SMG_Display(smgCode, smgCode + 7);
                // 月闪烁
                smgCode[3] = 17;
                smgCode[4] = 17;
                j = 7;
                while (j--)
                    SMG_Display(smgCode, smgCode + 7);
            }
            else {
                // 日
                j = 7;
                while (j--)
                    SMG_Display(smgCode, smgCode + 7);
                // 日闪烁
                smgCode[6] = 17;
                smgCode[7] = 17;
                j = 7;
                while (j--)
                    SMG_Display(smgCode, smgCode + 7);
            }
        }
        else {
            smgCode[0] = setHour / 10;
            smgCode[1] = setHour % 10;
            smgCode[2] = 18;
            smgCode[3] = setMinite / 10;
            smgCode[4] = setMinite % 10;
            smgCode[5] = 18;
            smgCode[6] = setSecond / 10;
            smgCode[7] = setSecond % 10;

            if (i == 3) {
                // 秒
                j = 7;
                while (j--) {
                    SMG_Display(smgCode, smgCode + 7);
                    // 点
                    smgFuncs[2]();
                    SMG = smgNum[16];
                    delayMs();
                    SMG = 0x00;
                    smgFuncs[5]();
                    SMG = smgNum[16];
                    delayMs();
                    SMG = 0x00;
                }
                // 秒闪烁
                smgCode[0] = 17;
                smgCode[1] = 17;
                j = 7;
                while (j--) {
                    SMG_Display(smgCode, smgCode + 7);
                    // 点
                    smgFuncs[2]();
                    SMG = smgNum[16];
                    delayMs();
                    SMG = 0x00;
                    smgFuncs[5]();
                    SMG = smgNum[16];
                    delayMs();
                    SMG = 0x00;
                }
            }
            else if (i == 4) {
                // 分
                j = 7;
                while (j--) {
                    SMG_Display(smgCode, smgCode + 7);
                    // 点
                    smgFuncs[2]();
                    SMG = smgNum[16];
                    delayMs();
                    SMG = 0x00;
                    smgFuncs[5]();
                    SMG = smgNum[16];
                    delayMs();
                    SMG = 0x00;
                }
                // 分闪烁
                smgCode[3] = 17;
                smgCode[4] = 17;
                j = 7;
                while (j--) {
                    SMG_Display(smgCode, smgCode + 7);
                    // 点
                    smgFuncs[2]();
                    SMG = smgNum[16];
                    delayMs();
                    SMG = 0x00;
                    smgFuncs[5]();
                    SMG = smgNum[16];
                    delayMs();
                    SMG = 0x00;
                }
            }
            else {
                // 时
                j = 7;
                while (j--) {
                    SMG_Display(smgCode, smgCode + 7);
                    // 点
                    smgFuncs[2]();
                    SMG = smgNum[16];
                    delayMs();
                    SMG = 0x00;
                    smgFuncs[5]();
                    SMG = smgNum[16];
                    delayMs();
                    SMG = 0x00;
                }
                // 时闪烁
                smgCode[6] = 17;
                smgCode[7] = 17;
                j = 7;
                while (j--) {
                    SMG_Display(smgCode, smgCode + 7);
                    // 点
                    smgFuncs[2]();
                    SMG = smgNum[16];
                    delayMs();
                    SMG = 0x00;
                    smgFuncs[5]();
                    SMG = smgNum[16];
                    delayMs();
                    SMG = 0x00;
                }
            }
        }

        // 按键扫描
        keyRes = SKEY_Scan();
        switch (keyRes) {
        case SKEY1_PRESSED:
            i++;
            break;

        case SKEY2_PRESSED:
            if (i == 0) {
                if (setYear < 99)
                    setYear++;
            }
            else if (i == 1) {
                if (setMonth < 12)
                    setMonth++;
            }
            else if (i == 2) {
                if (setDay < 31)
                    setDay++;
            }
            else if (i == 3) {
                if (setHour < 23)
                    setHour++;
            }
            else if (i == 4) {
                if (setMinite < 59)
                    setMinite++;
            }
            else {
                if (setSecond < 59)
                    setSecond++;
            }

            break;

        case SKEY3_PRESSED:
            if (i == 0) {
                if (setYear > 20)
                    setYear--;
            }
            else if (i == 1) {
                if (setMonth > 1)
                    setMonth--;
            }
            else if (i == 2) {
                if (setDay > 1)
                    setDay--;
            }
            else if (i == 3) {
                if (setHour > 0)
                    setHour--;
            }
            else if (i == 4) {
                if (setMinite > 0)
                    setMinite--;
            }
            else {
                if (setSecond > 0)
                    setSecond--;
            }
            break;

        default:
            break;
        }
    }

    // ds1302赋初值
    gDS1302_Time[0] = ((setSecond / 10) << 4) + (setSecond % 10); // 秒
    gDS1302_Time[1] = ((setMinite / 10) << 4) + (setMinite % 10); // 分
    gDS1302_Time[2] = ((setHour / 10) << 4) + (setHour % 10);     // 时
    gDS1302_Time[3] = ((setDay / 10) << 4) + (setDay % 10);       // 日
    gDS1302_Time[4] = ((setMonth / 10) << 4) + (setMonth % 10);   // 月
    gDS1302_Time[6] = ((setYear / 10) << 4) + (setYear % 10);     // 年
}

// 温度计算
void Temp_Calculate(u16* u16data, float* temperature, u8* negFlag) {
    // 温度负数修正
    if ((*u16data) & 0xF800) {
        *u16data = (~(*u16data)) + 1;
        *negFlag = 1;
    }

    *temperature = (*u16data) * 0.0625;
}
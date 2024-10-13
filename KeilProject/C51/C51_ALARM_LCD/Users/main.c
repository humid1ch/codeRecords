#include "reg52.h"
#include "util.h"
#include "ds1302.h"
#include "ds18b20.h"
#include "lcd1602.h"
#include "beep.h"
#include "skey.h"

extern u8 gDS1302_Time[7];

// 用于记录确认闹钟设定时的时间
u8 alarmSet = 0;
u8 alarmHour = 0;
u8 alarmMinite = 0;
u8 alarmSecond = 0;

u8 setHour = 0;   // 用于小时设置, 最大23
u8 setMinite = 0; // 用于分钟设置, 最大59
u8 setSecond = 0; // 用于秒设置, 最大59
u8 setYear = 20;  // 用于年设置, 初始为20, 最大99
u8 setMonth = 0;  // 用于月设置, 最大为12
u8 setDay = 0;    // 用于日设置, 最大为31
u8 setWeek = 0;   // 用于周几设置, 最大为7

// 是否进行初始时间设置
void Set_Select();
// 初始时间设置
void Time_Set();
// 温度计算
void Temp_Calculate(u16* u16data, float* temperature, u8* negFlag);
// 时间显示
void LCD1602_Time_Display();
// 温度显示
void LCD1602_Temp_Display(float temp, u8 neg);
// 闹钟结束判断
void Alarm_Beep();
// 闹钟设置
void Alarm_Set();
// 闹钟
void Alarm();

void main() {
    u16 u16tempData = 0; // 接收 ds18b20 获取的值
    u8 negFlag = 0;
    float temperature = 0.0;

    BEEP_Init();
    LCD1602_Init();
    SKEY_Init();
    Set_Select();
    LCD1602_Clear();
    DS1302_Init();
    DS18B20_Init();

    while (1) {
        DS1302_GetTime();
        LCD1602_Time_Display();
        Alarm_Beep();
        delayNMs(20);

        DS18B20_ReadTemperature(&u16tempData);
        Temp_Calculate(&u16tempData, &temperature, &negFlag);
        LCD1602_Temp_Display(temperature, negFlag);
        negFlag = 0;

        if (SKEY_Scan() == SKEY4_PRESSED)
            Alarm();
    }
}

void stringsClear(char* str, u8 sizes) {
    u8 i = 0;
    for (i = 0; i < sizes; i++)
        str[i] = 0;
}

// 本闹钟思路, 不采用定时器, 而是 与DS1302读取数据做对比
// 采用定时器也可以, 可以将定时的时间均转换为秒, 设定定时器时间
void Alarm_Set() {
    u8 keyRes = 0;
    u8 setOK = 0;
    u8 alarmSecondCnt = 0; // 用于闹钟秒计时, 最大59
    u8 alarmMiniteCnt = 0; // 用于闹钟分钟计时, 最大59
    u8 alarmHourCnt = 0;   // 用于闹钟小时计时, 最大23
    char alarmSetStr[17] = {0};
    char setTips[17] = "K1:OK K2:+ K3:-";
    // 秒
    while (!setOK) {
        alarmSetStr[0] = 'S';
        alarmSetStr[1] = 'e';
        alarmSetStr[2] = 'c';
        alarmSetStr[3] = 'o';
        alarmSetStr[4] = 'n';
        alarmSetStr[5] = 'd';
        alarmSetStr[6] = ' ';
        alarmSetStr[7] = alarmSecondCnt / 10 + '0';
        alarmSetStr[8] = alarmSecondCnt % 10 + '0';
        LCD1602_FirstLine(alarmSetStr);
        LCD1602_SecondLine(setTips);
        keyRes = SKEY_Scan();
        switch (keyRes) {
        case SKEY1_PRESSED:
            setOK = 1;
            break;

        case SKEY2_PRESSED:
            alarmSecondCnt++;
            break;

        case SKEY3_PRESSED:
            alarmSecondCnt--;
            break;
        }
    }
    stringsClear(alarmSetStr, 17);
    LCD1602_Clear();

    // 分
    setOK = 0;
    while (!setOK) {
        alarmSetStr[0] = 'M';
        alarmSetStr[1] = 'i';
        alarmSetStr[2] = 'n';
        alarmSetStr[3] = 'i';
        alarmSetStr[4] = 't';
        alarmSetStr[5] = 'e';
        alarmSetStr[6] = ' ';
        alarmSetStr[7] = alarmMiniteCnt / 10 + '0';
        alarmSetStr[8] = alarmMiniteCnt % 10 + '0';
        LCD1602_FirstLine(alarmSetStr);
        LCD1602_SecondLine(setTips);
        keyRes = SKEY_Scan();
        switch (keyRes) {
        case SKEY1_PRESSED:
            setOK = 1;
            break;

        case SKEY2_PRESSED:
            if (alarmMiniteCnt < 59)
                alarmMiniteCnt++;
            break;

        case SKEY3_PRESSED:
            if (alarmMiniteCnt > 0)
                alarmMiniteCnt--;
            break;
        }
    }
    stringsClear(alarmSetStr, 17);
    LCD1602_Clear();

    // 时
    setOK = 0;
    while (!setOK) {
        alarmSetStr[0] = 'H';
        alarmSetStr[1] = 'o';
        alarmSetStr[2] = 'u';
        alarmSetStr[3] = 'r';
        alarmSetStr[4] = ' ';
        alarmSetStr[5] = alarmHourCnt / 10 + '0';
        alarmSetStr[6] = alarmHourCnt % 10 + '0';
        LCD1602_FirstLine(alarmSetStr);
        LCD1602_SecondLine(setTips);
        keyRes = SKEY_Scan();
        switch (keyRes) {
        case SKEY1_PRESSED:
            setOK = 1;
            break;

        case SKEY2_PRESSED:
            if (alarmHourCnt < 99)
                alarmHourCnt++;
            break;

        case SKEY3_PRESSED:
            if (alarmHourCnt > 0)
                alarmHourCnt--;
            break;
        }
    }
    stringsClear(alarmSetStr, 17);
    LCD1602_Clear();

    // 已设置闹钟
    alarmSet = 1;

    // 提示闹钟设置完成
    DS1302_GetTime(); // 获取当前时间
    alarmHour = (gDS1302_Time[2] >> 4) * 10 + (gDS1302_Time[2] & 0x0F);
    alarmMinite = (gDS1302_Time[1] >> 4) * 10 + (gDS1302_Time[1] & 0x0F);
    alarmSecond = (gDS1302_Time[0] >> 4) * 10 + (gDS1302_Time[0] & 0x0F);

    // 提示设置完成
    // Alarm: xx:xx:xx
    alarmSetStr[0] = 'A';
    alarmSetStr[1] = 'l';
    alarmSetStr[2] = 'a';
    alarmSetStr[3] = 'r';
    alarmSetStr[4] = 'm';
    alarmSetStr[5] = ':';
    alarmSetStr[6] = ' ';
    alarmSetStr[7] = alarmHourCnt / 10 + '0';
    alarmSetStr[8] = alarmHourCnt % 10 + '0';
    alarmSetStr[9] = ':';
    alarmSetStr[10] = alarmMiniteCnt / 10 + '0';
    alarmSetStr[11] = alarmMiniteCnt % 10 + '0';
    alarmSetStr[12] = ':';
    alarmSetStr[13] = alarmSecondCnt / 10 + '0';
    alarmSetStr[14] = alarmSecondCnt % 10 + '0';
    // 提示闹钟开始时间
    // Start:
    setTips[0] = 'S';
    setTips[1] = 't';
    setTips[2] = 'a';
    setTips[3] = 'r';
    setTips[4] = 't';
    setTips[5] = ':';
    setTips[6] = ' ';
    setTips[7] = alarmHour / 10 + '0';
    setTips[8] = alarmHour % 10 + '0';
    setTips[9] = ':';
    setTips[10] = alarmMinite / 10 + '0';
    setTips[11] = alarmMinite % 10 + '0';
    setTips[12] = ':';
    setTips[13] = alarmSecond / 10 + '0';
    setTips[14] = alarmSecond % 10 + '0';
    LCD1602_FirstLine(alarmSetStr);
    LCD1602_SecondLine(setTips);

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

    delayNMs(3000);
    LCD1602_Clear();
}

void Alarm() {
    char alarmAskStr[] = "Set Alarm?";
    char confirmStr[] = "K1: Yes  K2: No";
    LCD1602_Clear();
    LCD1602_FirstLine(alarmAskStr);
    LCD1602_SecondLine(confirmStr);

    while (1) {
        if (SKEY_Scan() == SKEY1_PRESSED) {
            LCD1602_Clear();
            Alarm_Set();
            break;
        }
        else if (SKEY_Scan() == SKEY2_PRESSED) {
            LCD1602_Clear();
            break;
        }
    }
}

// 闹钟判断
void Alarm_Beep() {
    if (alarmSet && gDS1302_Time[0] == alarmSecond && gDS1302_Time[1] == alarmMinite && gDS1302_Time[2] == alarmHour) {
        alarmSet = 0;
        BEEP_On(2);
    }
}

void LCD1602_Temp_Display(float temp, u8 neg) {
    u16 tempInt = (u16)(temp *= 10);
    u8 mod = 0;
    // -xx.xC
    // 124
    char tempCode[7] = {0};
    u8 i = 0;

    char cnt = 2;
    while (tempInt) {
        mod = tempInt % 10;
        tempInt /= 10;
        tempCode[cnt--] = mod + '0';
    }
    // xxx000 -> 0xx0x0
    tempCode[4] = tempCode[2];
    tempCode[2] = tempCode[1];
    tempCode[1] = tempCode[0];
    // 0xx0x0 加上符号
    if (neg)
        tempCode[0] = '-';
    else
        tempCode[0] = ' ';

    tempCode[3] = '.';
    tempCode[5] = 'C';

    // 第二行的最后六位
    for (i = 0; i < 6; i++)
        LCD1602_Dispaly(0x4A + i, tempCode[i]);
}

void Set_Select() {
    char setAskStr[] = "Set Default Time";
    char confirmStr[] = "K1: Yes  K2: No";
    LCD1602_FirstLine(setAskStr);
    LCD1602_SecondLine(confirmStr);

    while (1) {
        if (SKEY_Scan() == SKEY1_PRESSED) {
            LCD1602_Clear();
            Time_Set();
            break;
        }
        else if (SKEY_Scan() == SKEY2_PRESSED) {
            LCD1602_Clear();
            break;
        }
    }
}

void LCD1602_Time_Display() {
    u8 strData[13] = {0};
    u8 strTime[9] = {0};
    u8 u8data = 0;
    u8 i = 0;

    // xx-xx-xx
    for (i = 0; i < 3; i++) {
        u8data = (gDS1302_Time[i] >> 4) * 10 + (gDS1302_Time[i] & 0x0F);
        strTime[(2 - i) * 3] = u8data / 10 + '0';
        strTime[(2 - i) * 3 + 1] = u8data % 10 + '0';
    }
    strTime[2] = ':';
    strTime[5] = ':';
    // 0123456789
    // xx-xx-xx-x
    // 日
    u8data = (gDS1302_Time[i] >> 4) * 10 + (gDS1302_Time[i] & 0x0F);
    strData[8] = u8data / 10 + '0';
    strData[9] = u8data % 10 + '0';
    i++;
    // 月
    u8data = (gDS1302_Time[i] >> 4) * 10 + (gDS1302_Time[i] & 0x0F);
    strData[5] = u8data / 10 + '0';
    strData[6] = u8data % 10 + '0';
    i++;
    // 周
    u8data = (gDS1302_Time[i] >> 4) * 10 + (gDS1302_Time[i] & 0x0F);
    strData[11] = u8data % 10 + '0';
    i++;
    // 年
    u8data = (gDS1302_Time[i] >> 4) * 10 + (gDS1302_Time[i] & 0x0F);
    strData[0] = '2';
    strData[1] = '0';
    strData[2] = u8data / 10 + '0';
    strData[3] = u8data % 10 + '0';
    strData[4] = '-';
    strData[7] = '-';
    strData[10] = '-';

    LCD1602_FirstLine(strData);
    LCD1602_SecondLine(strTime);
}

void Time_Set() {
    char str[9] = {0};
    u8 confirmFlag = 0;
    u8 keyRes = 0;
    char setTips[17] = "K1:OK K2:+ K3:-";
    // 年的设置
    while (!confirmFlag) {
        str[0] = 'Y';
        str[1] = 'e';
        str[2] = 'a';
        str[3] = 'r';
        str[4] = ' ';

        keyRes = SKEY_Scan();
        switch (keyRes) {
        case SKEY1_PRESSED:
            confirmFlag = 1;
            break;

        case SKEY2_PRESSED:
            if (setYear < 99)
                setYear++;
            break;

        case SKEY3_PRESSED:
            if (setYear > 20)
                setYear--;
            break;

        default:
            break;
        }

        str[5] = setYear / 10 + '0';
        str[6] = setYear % 10 + '0';

        LCD1602_FirstLine(str);
        LCD1602_SecondLine(setTips);
    }
    stringsClear(str, 9);
    LCD1602_Clear();

    // 月的设置
    confirmFlag = 0;
    while (!confirmFlag) {
        str[0] = 'M';
        str[1] = 'o';
        str[2] = 'n';
        str[3] = 't';
        str[4] = 'h';
        str[5] = ' ';

        keyRes = SKEY_Scan();
        switch (keyRes) {
        case SKEY1_PRESSED:
            confirmFlag = 1;
            break;

        case SKEY2_PRESSED:
            if (setMonth < 12)
                setMonth++;
            break;

        case SKEY3_PRESSED:
            if (setMonth > 0)
                setMonth--;
            break;

        default:
            break;
        }

        str[6] = setMonth / 10 + '0';
        str[7] = setMonth % 10 + '0';

        LCD1602_FirstLine(str);
        LCD1602_SecondLine(setTips);
    }
    stringsClear(str, 9);
    LCD1602_Clear();

    // 日的设置
    confirmFlag = 0;
    while (!confirmFlag) {
        str[0] = 'D';
        str[1] = 'a';
        str[2] = 'y';
        str[3] = ' ';

        keyRes = SKEY_Scan();
        switch (keyRes) {
        case SKEY1_PRESSED:
            confirmFlag = 1;
            break;

        case SKEY2_PRESSED:
            if (setDay < 31)
                setDay++;
            break;

        case SKEY3_PRESSED:
            if (setDay > 0)
                setDay--;
            break;

        default:
            break;
        }

        str[4] = setDay / 10 + '0';
        str[5] = setDay % 10 + '0';

        LCD1602_FirstLine(str);
        LCD1602_SecondLine(setTips);
    }
    stringsClear(str, 9);
    LCD1602_Clear();

    // 周的设置
    confirmFlag = 0;
    while (!confirmFlag) {
        str[0] = 'W';
        str[1] = 'e';
        str[2] = 'e';
        str[3] = 'k';
        str[4] = ' ';

        keyRes = SKEY_Scan();
        switch (keyRes) {
        case SKEY1_PRESSED:
            confirmFlag = 1;
            break;

        case SKEY2_PRESSED:
            if (setWeek < 7)
                setWeek++;
            break;

        case SKEY3_PRESSED:
            if (setWeek > 0)
                setWeek--;
            break;

        default:
            break;
        }

        str[5] = setWeek / 10 + '0';
        str[6] = setWeek % 10 + '0';

        LCD1602_FirstLine(str);
        LCD1602_SecondLine(setTips);
    }
    stringsClear(str, 9);
    LCD1602_Clear();

    // 小时的设置
    confirmFlag = 0;
    while (!confirmFlag) {
        str[0] = 'H';
        str[1] = 'o';
        str[2] = 'u';
        str[3] = 'r';
        str[4] = ' ';

        keyRes = SKEY_Scan();
        switch (keyRes) {
        case SKEY1_PRESSED:
            confirmFlag = 1;
            break;

        case SKEY2_PRESSED:
            if (setHour < 23)
                setHour++;
            break;

        case SKEY3_PRESSED:
            if (setHour > 0)
                setHour--;
            break;

        default:
            break;
        }

        str[5] = setHour / 10 + '0';
        str[6] = setHour % 10 + '0';

        LCD1602_FirstLine(str);
        LCD1602_SecondLine(setTips);
    }
    stringsClear(str, 9);
    LCD1602_Clear();

    // 分钟的设置
    confirmFlag = 0;
    while (!confirmFlag) {
        str[0] = 'M';
        str[1] = 'i';
        str[2] = 'n';
        str[3] = 'i';
        str[4] = 't';
        str[5] = 'e';
        str[6] = ' ';

        keyRes = SKEY_Scan();
        switch (keyRes) {
        case SKEY1_PRESSED:
            confirmFlag = 1;
            break;

        case SKEY2_PRESSED:
            if (setMinite < 59)
                setMinite++;
            break;

        case SKEY3_PRESSED:
            if (setMinite > 0)
                setMinite--;
            break;

        default:
            break;
        }

        str[7] = setMinite / 10 + '0';
        str[8] = setMinite % 10 + '0';

        LCD1602_FirstLine(str);
        LCD1602_SecondLine(setTips);
    }
    stringsClear(str, 9);
    LCD1602_Clear();

    // 秒的设置
    confirmFlag = 0;
    while (!confirmFlag) {
        str[0] = 'S';
        str[1] = 'e';
        str[2] = 'c';
        str[3] = 'o';
        str[4] = 'n';
        str[5] = 'd';
        str[6] = ' ';

        keyRes = SKEY_Scan();
        switch (keyRes) {
        case SKEY1_PRESSED:
            confirmFlag = 1;
            break;

        case SKEY2_PRESSED:
            if (setSecond < 59)
                setSecond++;
            break;

        case SKEY3_PRESSED:
            if (setSecond > 0)
                setSecond--;
            break;

        default:
            break;
        }

        str[7] = setSecond / 10 + '0';
        str[8] = setSecond % 10 + '0';

        LCD1602_FirstLine(str);
        LCD1602_SecondLine(setTips);
    }
    stringsClear(str, 9);
    LCD1602_Clear();

    // ds1302赋初值
    gDS1302_Time[0] = ((setSecond / 10) << 4) + (setSecond % 10); // 秒
    gDS1302_Time[1] = ((setMinite / 10) << 4) + (setMinite % 10); // 分
    gDS1302_Time[2] = ((setHour / 10) << 4) + (setHour % 10);     // 时
    gDS1302_Time[3] = ((setDay / 10) << 4) + (setDay % 10);       // 日
    gDS1302_Time[4] = ((setMonth / 10) << 4) + (setMonth % 10);   // 月
    gDS1302_Time[5] = ((setWeek / 10) << 4) + (setWeek % 10);     // 周
    gDS1302_Time[6] = ((setYear / 10) << 4) + (setYear % 10);     // 年
}

void Temp_Calculate(u16* u16data, float* temperature, u8* negFlag) {
    // 温度负数修正
    if ((*u16data) & 0xF800) {
        *u16data = (~(*u16data)) + 1;
        *negFlag = 1;
    }

    *temperature = (*u16data) * 0.0625;
}

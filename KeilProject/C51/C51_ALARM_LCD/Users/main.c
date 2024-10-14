#include "reg52.h"
#include "util.h"
#include "ds1302.h"
#include "ds18b20.h"
#include "lcd1602.h"
#include "beep.h"
#include "skey.h"

// DS1302 日期时间数据存放数组
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
u8 setMonth = 1;  // 用于月设置, 最大为12
u8 setDay = 1;    // 用于日设置, 最大为31
u8 setWeek = 1;   // 用于周几设置, 最大为7

// 字符串清空
void stringClear(char* str, u8 sizes);
// 字符串填充
void stringFill(char* str1, const char* str2);

// 日期时间 初始设置询问(开机询问)
void DefaultDate_Set_Ask();
// 日期时间 修改设置询问
void Date_Set_Ask();
// 初始时间设置
void Date_Set();

// 闹钟结束判断
void Alarm_Beep();
// 闹钟设置
void Alarm_Set();
// 闹钟 设置询问
void Alarm_Set_Ask();

// 温度计算
void Temp_Calculate(u16* u16data, float* temperature, u8* negFlag);
// 时间显示
void LCD1602_Time_Display();
// 温度显示
void LCD1602_Temp_Display(float temp, u8 neg);

void main() {
    u16 u16tempData = 0; // 接收 ds18b20 获取的值
    u8 negFlag = 0;
    float temperature = 0.0;

    BEEP_Init();           // 蜂鸣器初始化
    LCD1602_Init();        // LCD1602初始化
    SKEY_Init();           // 独立按键初始化
    DefaultDate_Set_Ask(); // 询问是否需要设置初始时间
    DS1302_Init();         // DS1302 时钟初始化
    DS18B20_Init();        // DS18B20 温度传感器初始化

    while (1) {
        DS1302_GetTime();       // DS1302 读取日期时间信息
        LCD1602_Time_Display(); // 显示日期时间信息
        Alarm_Beep();           // 判断闹钟是否响
        delayNMs(20);

        DS18B20_ReadTemperature(&u16tempData);                // 读取 16位温度量
        Temp_Calculate(&u16tempData, &temperature, &negFlag); // 计算具体温度
        LCD1602_Temp_Display(temperature, negFlag);           // 显示温度
        negFlag = 0;

        // 按键扫描, 判断K4是否被按下, 并询问是否进行 闹钟 和 日期时间的设置
        if (SKEY_Scan() == SKEY4_PRESSED) {
            Date_Set_Ask();
            LCD1602_Clear();
            Alarm_Set_Ask();
        }
    }
}

// 本闹钟思路, 不采用定时器, 而是 与DS1302读取数据做对比
// 采用定时器也可以, 可以将定时的时间均转换为秒, 设定定时器时间
void Alarm_Set() {
    u8 keyRes = 0;         // 记录按键扫描返回值
    u8 confirmFlag = 0;    // 记录确认
    u8 alarmSecondCnt = 0; // 用于闹钟秒计时, 最大59
    u8 alarmMiniteCnt = 0; // 用于闹钟分钟计时, 最大59
    u8 alarmHourCnt = 0;   // 用于闹钟小时计时, 最大23
    char alarmSetStr[16] = {0};
    char setTips[17] = "K1:OK K2:+ K3:-";

    u8 i = 0;
    // i: 0秒 1分 2时
    for (i = 0; i < 3; i++) {
        while (!confirmFlag) {
            if (i == 0) {
                // 秒
                stringFill(alarmSetStr, "Second ");
                alarmSetStr[7] = alarmSecondCnt / 10 + '0';
                alarmSetStr[8] = alarmSecondCnt % 10 + '0';
            }
            else if (i == 1) {
                // 分
                stringFill(alarmSetStr, "Minite ");
                alarmSetStr[7] = alarmMiniteCnt / 10 + '0';
                alarmSetStr[8] = alarmMiniteCnt % 10 + '0';
            }
            else {
                // 时
                stringFill(alarmSetStr, "Hour ");
                alarmSetStr[5] = alarmHourCnt / 10 + '0';
                alarmSetStr[6] = alarmHourCnt % 10 + '0';
            }
            LCD1602_FirstLine(alarmSetStr);
            LCD1602_SecondLine(setTips);

            keyRes = SKEY_Scan();
            switch (keyRes) {
            case SKEY1_PRESSED:
                confirmFlag = 1;
                break;

            case SKEY2_PRESSED:
                if (i == 0)
                    alarmSecondCnt++;
                else if (i == 1)
                    alarmMiniteCnt++;
                else
                    alarmHourCnt++;
                break;

            case SKEY3_PRESSED:
                if (i == 0)
                    alarmSecondCnt--;
                else if (i == 1)
                    alarmMiniteCnt--;
                else
                    alarmHourCnt--;
                break;
            }
        }
        stringClear(alarmSetStr, 9);
        LCD1602_Clear();
        confirmFlag = 0;
    }

    // 已设置闹钟
    alarmSet = 1;

    // 提示闹钟设置完成
    DS1302_GetTime(); // 获取当前时间
    alarmHour = (gDS1302_Time[2] >> 4) * 10 + (gDS1302_Time[2] & 0x0F);
    alarmMinite = (gDS1302_Time[1] >> 4) * 10 + (gDS1302_Time[1] & 0x0F);
    alarmSecond = (gDS1302_Time[0] >> 4) * 10 + (gDS1302_Time[0] & 0x0F);

    // 提示设置完成
    // Alarm: xx:xx:xx
    stringFill(alarmSetStr, "Alarm: ");
    alarmSetStr[7] = alarmHourCnt / 10 + '0';
    alarmSetStr[8] = alarmHourCnt % 10 + '0';
    alarmSetStr[9] = ':';
    alarmSetStr[10] = alarmMiniteCnt / 10 + '0';
    alarmSetStr[11] = alarmMiniteCnt % 10 + '0';
    alarmSetStr[12] = ':';
    alarmSetStr[13] = alarmSecondCnt / 10 + '0';
    alarmSetStr[14] = alarmSecondCnt % 10 + '0';

    // 提示闹钟开始时间
    // Start: xx:xx:xx
    stringFill(setTips, "Start: ");
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

void Alarm_Set_Ask() {
    char alarmAskStr[] = "Set Alarm?";
    char confirmStr[] = "K1: Yes  K2: No";
    u8 keyRes = 0;
    LCD1602_Clear();
    LCD1602_FirstLine(alarmAskStr);
    LCD1602_SecondLine(confirmStr);

    while (!keyRes) {
        keyRes = SKEY_Scan();
        switch (keyRes) {
        case SKEY1_PRESSED:
            LCD1602_Clear();
            Alarm_Set();
            break;

        case SKEY2_PRESSED:
            LCD1602_Clear();
            break;

        default:
            keyRes = 0;
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

// 时间设置询问
void DefaultDate_Set_Ask() {
    char setAskStr[] = "Set Default Time";
    char confirmStr[] = "K1: Yes  K2: No";
    u8 keyRes = 0;
    LCD1602_FirstLine(setAskStr);
    LCD1602_SecondLine(confirmStr);

    while (!keyRes) {
        keyRes = SKEY_Scan();
        switch (keyRes) {
        case SKEY1_PRESSED:
            LCD1602_Clear();
            Date_Set();
            break;

        case SKEY2_PRESSED:
            LCD1602_Clear();
            break;

        default:
            keyRes = 0;
            break;
        }
    }
    LCD1602_Clear(); // LCD清屏
}

// 时间设置函数
void Date_Set() {
    char timeSetStr[10] = {0};
    char setTips[17] = "K1:OK K2:+ K3:-";
    u8 confirmFlag = 0; // 记录确认
    u8 keyRes = 0;      // 记录按键扫描返回值

    u8 i = 0;
    // i: 0(年) 1(月) 2(日) 3(周) 4(时) 5(分) 6(秒)
    for (i = 0; i < 7; i++) {
        while (!confirmFlag) {
            if (i == 0) {
                // 年
                stringFill(timeSetStr, "Year ");
                timeSetStr[5] = setYear / 10 + '0';
                timeSetStr[6] = setYear % 10 + '0';
            }
            else if (i == 1) {
                // 月
                stringFill(timeSetStr, "Month ");
                timeSetStr[6] = setMonth / 10 + '0';
                timeSetStr[7] = setMonth % 10 + '0';
            }
            else if (i == 2) {
                // 日
                stringFill(timeSetStr, "Day ");
                timeSetStr[4] = setDay / 10 + '0';
                timeSetStr[5] = setDay % 10 + '0';
            }
            else if (i == 3) {
                // 周
                stringFill(timeSetStr, "Week ");
                timeSetStr[5] = setWeek / 10 + '0';
                timeSetStr[6] = setWeek % 10 + '0';
            }
            else if (i == 4) {
                // 时
                stringFill(timeSetStr, "Hour ");
                timeSetStr[5] = setHour / 10 + '0';
                timeSetStr[6] = setHour % 10 + '0';
            }
            else if (i == 5) {
                // 分
                stringFill(timeSetStr, "Minite ");
                timeSetStr[7] = setMinite / 10 + '0';
                timeSetStr[8] = setMinite % 10 + '0';
            }
            else {
                // 秒
                stringFill(timeSetStr, "Second ");
                timeSetStr[7] = setSecond / 10 + '0';
                timeSetStr[8] = setSecond % 10 + '0';
            }
            // 显示
            LCD1602_FirstLine(timeSetStr);
            LCD1602_SecondLine(setTips);

            // 按键扫描
            keyRes = SKEY_Scan();
            switch (keyRes) {
            case SKEY1_PRESSED:
                confirmFlag = 1;
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
                    if (setWeek < 7)
                        setWeek++;
                }
                else if (i == 4) {
                    if (setHour < 23)
                        setHour++;
                }
                else if (i == 5) {
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
                    if (setWeek > 1)
                        setWeek--;
                }
                else if (i == 4) {
                    if (setHour > 0)
                        setHour--;
                }
                else if (i == 5) {
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
        stringClear(timeSetStr, 9);
        LCD1602_Clear();

        confirmFlag = 0;
    }

    // ds1302赋初值
    gDS1302_Time[0] = ((setSecond / 10) << 4) + (setSecond % 10); // 秒
    gDS1302_Time[1] = ((setMinite / 10) << 4) + (setMinite % 10); // 分
    gDS1302_Time[2] = ((setHour / 10) << 4) + (setHour % 10);     // 时
    gDS1302_Time[3] = ((setDay / 10) << 4) + (setDay % 10);       // 日
    gDS1302_Time[4] = ((setMonth / 10) << 4) + (setMonth % 10);   // 月
    gDS1302_Time[5] = ((setWeek / 10) << 4) + (setWeek % 10);     // 周
    gDS1302_Time[6] = ((setYear / 10) << 4) + (setYear % 10);     // 年
}

void Date_Set_Ask() {
    char dateAskStr[] = "Set Date?";
    char confirmStr[] = "K1: Yes  K2: No";
    u8 keyRes = 0;
    LCD1602_Clear();
    LCD1602_FirstLine(dateAskStr);
    LCD1602_SecondLine(confirmStr);

    while (1) {
        keyRes = SKEY_Scan();
        if (keyRes == SKEY1_PRESSED) {
            LCD1602_Clear();
            Date_Set();    // 需要重新校准时间
            DS1302_Init(); // 校准之后要重新初始化
            break;
        }
        else if (keyRes == SKEY2_PRESSED) {
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

// 温度显示函数
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

void Temp_Calculate(u16* u16data, float* temperature, u8* negFlag) {
    // 温度负数修正
    if ((*u16data) & 0xF800) {
        *u16data = (~(*u16data)) + 1;
        *negFlag = 1;
    }

    *temperature = (*u16data) * 0.0625;
}

void stringClear(char* str, u8 sizes) {
    u8 i = 0;
    for (i = 0; i < sizes; i++)
        str[i] = 0;
}

void stringFill(char* str1, const char* str2) {
    u8 i = 0;
    while (*(str2 + i)) {
        *(str1 + i) = *(str2 + i);
        i++;
    }
}

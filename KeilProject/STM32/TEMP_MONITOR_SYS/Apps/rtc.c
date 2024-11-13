#include "rtc.h"
#include "stdio.h"

static const Date baseDate = {1970, 1, 1, 0, 0, 0};

static const u8 monthDays[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static u8 getWeek(const Date* nowDate);
static u8 isLeapYear(u16 year);
static u8 getMonthDay(u16 year, u8 month);
static u16 getLeapYears(u16 year);
static u32 getDateStampDiff(const Date* date, u16 timeZoneDiff);
static void getDateFromStamp(u32 timeStamp, Date* resultDate, u8 timeZoneDiff);

// 年-月-日 时:分:秒 周
Date nowDate = {2024, 11, 11, 10, 05, 0, 1};

// 参数:
// RTC需要设置的时间
u8 RTC_init(void) {
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); // 打开电源时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE); // 打开 RTC 后备域时钟

    PWR_BackupAccessCmd(ENABLE); // 打开后备寄存器访问

    // 从指定的备份域中读取数据，判断是否在备份域中写入过初始时间
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xAAAA) {
		BKP_DeInit(); // 备分域复位
        RCC_LSEConfig(RCC_LSE_ON); // 开启外部 32.768K RTC 时钟

        u8 count = 0;
        // 检查LSE是否启振成功
        while ((RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) && count < 250) {
            count++;
            delayMs(10);
        }
        if (count > 250)
            return 1; // 初始化时钟失败，晶振有问题

        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); // 选择 LSE 作为 RTC 时钟
        RCC_RTCCLKCmd(ENABLE);                  // 使能 RTC 时钟
        RTC_WaitForSynchro();                   // 等待上一次对RTC寄存器的写操作完成
        RTC_WaitForLastTask();                  // 等待最后一次对RTC寄存器的写操作完成

        RTC_EnterConfigMode();   // 允许配置
        RTC_SetPrescaler(32768); // 设置RTC预分频值
        RTC_WaitForLastTask();   // 等待最后一次对RTC寄存器的写操作完成

        // 设置RTC 默认时间(不是基准时间, 基准时间恒为1970-1-1 00:00:00)
        RTC_set(&nowDate);
        RTC_ExitConfigMode(); //退出配置模式，

        BKP_WriteBackupRegister(BKP_DR1, 0xAAAA); //向指定的后备域寄存器写入用户标志数据
    }

    RTC_WaitForSynchro(); // 等待上一次对RTC寄存器的写操作完成
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    RTC_WaitForLastTask(); // 等待最后一次对RTC寄存器的写操作完成

    // 中断模块初始化
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);

    RTC_get(&nowDate); // 从RTC中获取时间

    return 0;
}

// RTC时钟中断
// 每秒触发一次, 即每秒更新一次 时间
void RTC_IRQHandler(void) {
    if (RTC_GetITStatus(RTC_IT_SEC) == SET)
        RTC_get(&nowDate);

    RTC_ClearITPendingBit(RTC_IT_SEC);
    RTC_WaitForLastTask(); // 等待最后一次对RTC寄存器的写操作完成
}

// 向RTC中设置初始时间
u8 RTC_set(const Date* defaultDate) {
    u32 timeStamp = getDateStampDiff(defaultDate, 8);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //使能PWR和BKP外设时钟
    PWR_BackupAccessCmd(ENABLE);                                             //使能RTC和后备寄存器访问

    RTC_SetCounter(timeStamp); // 设置RTC计数器的值

    RTC_WaitForLastTask(); // 等待最近一次对RTC寄存器的写操作完成

    return 0;
}

// 从RTC中获取时间, 获取到的时间 由传入的参数输出
u8 RTC_get(Date* nowDates) {
    u32 timecount = 0;

    timecount = RTC_GetCounter(); // 从RTC备份域中把CNT寄存器的值读出来(时间戳)

    getDateFromStamp(timecount, nowDates, 8);

    nowDates->week = getWeek(nowDates); //获取星期

    return 0;
}

// 蔡勒公式计算, 星期几
static u8 getWeek(const Date* nowDate) {
    u8 month = nowDate->month;
    u8 year = nowDate->year;
    u32 day = nowDate->day;

    if (month < 3) {
        month += 12;
        year -= 1;
    }

    int yearL = year % 100;
    int yearH = year / 100;
    int week = (day + (13 * (month + 1)) / 5 + yearL + yearL / 4 + yearH / 4 + 5 * yearH) % 7;

    return week;
}

// 判断闰年
u8 isLeapYear(u16 year) {
    // 能被四百整除 或 能被四整除不能被100整除
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        return 1;

    return 0;
}

u8 getMonthDay(u16 year, u8 month) {
    u8 result = monthDays[month];

    if (isLeapYear(year) && month == 2)
        result = 29;

    return result;
}

// 计算相对基准年份 之间有多少闰年, [1970 ~ n)
u16 getLeapYears(u16 year) {
    u16 result = 0;
    for (u16 i = baseDate.year + 1; i < year; i++) {
        if (isLeapYear(i))
            result++;
    }

    return result;
}

// 简单的计算思路:
// 基准日期: 1970-1-1 00:00:00
// 给定日期: 2024-11-2 22:22:22
// 1. 计算之间完整的年, 即 1971 ~ 2023
// 2. 计算基准日期 到 其年末 完整的天, 即 1-2 ~ 12-31 (1-1不完整)
// 3. 计算给定日期 距 其年初 完整的天, 即 1-1 ~ 11-1 (11-2没过完)
// 4. 结合完整的年 和 完整的天, 计算出 两个日期之间完整的天数
// 5. 计算 基准日期当天, 还有多少秒过完
// 6. 计算 给定日期当天, 已经过了多少秒
// 7. 最终结合完整的天 以及 5 6步计算的秒, 能计算出两日期之间一共过了多少秒
u32 getDateStampDiff(const Date* date, u16 timeZoneDiff) {
    u32 result = 0;
    u16 leapYearNum = getLeapYears(date->year);

    // 计算两个日期的年差 (只计算完整一年的个数)
    int yearDiff = date->year - (baseDate.year + 1);
    if (yearDiff < 0) // 基准年和给定年 实际在同一年
        yearDiff = 0;

    // 计算基准日期 到年末 完整的天数
    u16 sumDay = 0;
    for (u8 i = baseDate.month + 1; i <= 12; i++)
        sumDay += monthDays[i];
    sumDay += (monthDays[baseDate.month] - baseDate.day);
    // 闰年补差
    if (isLeapYear(baseDate.year) && ((baseDate.month == 2 && baseDate.day < 29) || (baseDate.month < 2)))
        sumDay++;

    // 计算给定日期 距年初 完整的天数
    for (u8 i = 1; i < date->month; i++)
        sumDay += monthDays[i];
    sumDay += (date->day - 1);
    // 闰年补差
    if (isLeapYear(date->year) && date->month > 2)
        sumDay++;

    // 计算天数和
    sumDay += (yearDiff * 365) + leapYearNum;

    // 计算 基准日期当天还有多少秒结束, 和 给定日期当天已经过了多少秒
    u32 sumSecond = 0;
    // 1. 基准日期当天还有多少秒结束
    sumSecond = (24 - (baseDate.hour + 1)) * 60 * 60 + (60 - (baseDate.minute + 1)) * 60 + (60 - baseDate.second);
    // 2. 给定日期当天已经过了多少秒
    sumSecond += date->hour * 60 * 60 + date->minute * 60 + date->second;

    result += sumSecond + sumDay * 24 * 60 * 60;

    // 时区矫正
    result -= timeZoneDiff * 60 * 60;

    return result;
}

// 通过给定时间戳, 计算从基准值 经历 这些时间戳之后的日期
// 简单的计算思路
// 1. 计算基准日期 当天还有多少秒过完, 并从 给定的时间戳中减去
// 2. 从时间戳的剩余值中计算 还能有多少完整的天
void getDateFromStamp(u32 timeStamp, Date* resultDate, u8 timeZoneDiff) {
    if (resultDate == NULL)
        return;

    timeStamp += (timeZoneDiff * 60 * 60);

    resultDate->year = baseDate.year;
    resultDate->month = baseDate.month;
    resultDate->day = baseDate.day;
    resultDate->hour = baseDate.hour;
    resultDate->minute = baseDate.minute;
    resultDate->second = baseDate.second;
    u32 baseDaySecond = (24 - (resultDate->hour + 1)) * 60 * 60 + (60 - (resultDate->minute + 1)) * 60 + (60 - resultDate->second);
    timeStamp -= baseDaySecond;
    resultDate->day++;
    resultDate->hour = 0;
    resultDate->minute = 0;
    resultDate->second = 0;

    // 剩下的时间戳有多少完整的天 20030
    u32 dayNum = timeStamp / (24 * 60 * 60);
    // 最后的秒数, 即 一天已经过了的秒数
    u32 remainSecond = timeStamp % (24 * 60 * 60);

    // 计算 从基准日期 过了 dayNum 之后的日子
    resultDate->day += dayNum;
    while (resultDate->day > getMonthDay(resultDate->year, resultDate->month)) {
        resultDate->day -= getMonthDay(resultDate->year, resultDate->month);
        resultDate->month++;
        if (resultDate->month == 13) {
            resultDate->year++;
            resultDate->month = 1;
        }
    }

    resultDate->hour = remainSecond / (60 * 60);
    remainSecond %= (60 * 60);
    resultDate->minute = remainSecond / 60;
    remainSecond %= 60;
    resultDate->second = remainSecond;
}

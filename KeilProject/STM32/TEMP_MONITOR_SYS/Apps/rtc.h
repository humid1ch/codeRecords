#ifndef __RTC_H__
#define __RTC_H__

#include "stm32f10x.h"
#include "utils.h"

// 参数: 年 月 日 时 分 秒
// 计算相对于 1970-1-1 00:00:00
// 结果为 秒
typedef struct Date {
    u16 year;
    u8 month;
    u32 day;
    u8 hour;
    u8 minute;
    u8 second;
    u8 week;
} Date;

extern Date nowDate;

u8 RTC_init(void);                   // 初始化RTC,返回1,失败; 返回0,成功;
u8 RTC_get(Date* nowDates);          // 从RTC中获取时间
u8 RTC_set(const Date* defaultDate); // 向RTC中设置初始时间

#endif

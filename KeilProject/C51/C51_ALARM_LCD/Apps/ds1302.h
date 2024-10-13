#ifndef __DS1302_H__
#define __DS1302_H__

#include "util.h"

extern u8 gDS1302_Time[7];

/* 使用控制寄存器 选择寄存器需发送的数据 */
// 写控制, 选择寄存器之后 |, 选择写模式
#define REG_W_CTRL 0x00
// 读控制, 选择寄存器之后 |, 选择读模式
#define REG_R_CTRL 0x01

// 秒寄存器 1000 000x
#define SECONDS_CTRL 0x80
// 分寄存器 1000 001x
#define MINITES_CTRL 0x82
// 时寄存器 1000 010x
#define HOURS_CTRL 0x84
// 日寄存器 1000 011x
#define DAYS_CTRL 0x86
// 月寄存器 1000 100x
#define MONTHES_CTRL 0x88
// 周(周几)寄存器 1000 101x
#define WEEKS_CTRL 0x8A
// 年寄存器 1000 110x
#define YEARS_CTRL 0x8C
// 写保护寄存器 1000 111x
#define WRITE_PRO_CTRL 0x8E

void DS1302_Init();

void DS1302_GetTime();

#endif
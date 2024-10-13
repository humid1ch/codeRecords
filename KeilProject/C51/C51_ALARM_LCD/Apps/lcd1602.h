#ifndef __LCD1602_H__
#define __LCD1602_H__

#include "reg52.h"
#include "util.h"

#define LCD_DATA P0
// 清屏
#define CLEAR_CMD 0x01
// 光标移动模式 和 显示器移动模式设置
#define MODE_SET 0x04
// 写入后, 光标左移
#define CURSOR_L 0x00
// 写入后, 光标右移
#define CURSOR_R 0x02
// 写入后, 显示器不动
#define DISPLAY_STOP 0x00
// 写入后, 显示器右移一字符
#define DISPLAY_R 0x01

// 显示器 光标开关 和 光标闪烁设置
#define SWITCH_SET 0x08
// 显示器开关
#define DISPLAY_ON 0x04
#define DISPLAY_OFF 0x00
// 光标开关
#define CURSOR_ON 0x02
#define CURSOR_OFF 0x00
// 光标闪烁开关 打开闪烁 会影响光标开关设置
#define BLINK_ON 0x01
#define BLINK_OFF 0x00

// 功能设置, 显示行数 数据总线宽度 字符分辨率
#define FUNC_SET 0x20
// 数据总线位数设置
#define DATA_LINE_4BIT 0x00
#define DATA_LINE_8BIT 0x10
// 显示行数设置
#define ONE_LINE 0x00
#define TWO_LINE 0x08
// 字符分辨率(点阵)
#define CHAR_HIGH 0x04
#define CHAR_LOW 0x00

#define ADDR_SET 0x80

void LCD1602_Init();
void LCD1602_Clear();
void LCD1602_Dispaly(u8 u8addr, u8 u8data);
void LCD1602_FirstLine(const char* str);
void LCD1602_SecondLine(const char* str);

#endif
#ifndef _LCD9648_H
#define _LCD9648_H

#include "util.h"

//LCD显示范围
#define LCD_WIDTH 96
#define LCD_HEIGHT 48

//函数声明
void LCD9648_init();
void LCD9648_clear();
void LCD9648_showChinese(u8 x, u8 y, u8 font_wh, char* str);
void LCD9648_showChar(u8 x, u8 y, char c);
void LCD9648_showString(u8 x, u8 y, u8 len, const char* str);
void LCD9648_show16HZ(u8 x, u8 y, const u8* cn_map);
void LCD9648_showNum(u8 x, u8 y, u32 num, u8 len, u8 mode);
void LCD9648_showImage(u8* p);

#endif

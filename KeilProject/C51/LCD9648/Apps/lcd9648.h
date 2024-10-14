#ifndef _LCD9648_H
#define _LCD9648_H

#include "util.h"

//LCD显示范围
#define LCD_WIDTH 96
#define LCD_HEIGHT 48

//函数声明
void LCD9648_Init();
void LCD9648_Clear();
void LCD9648_WriteData(u8 u8data);
void LCD9648_WriteCmd(u8 u8cmd);
void LCD_SHOW_Char(u8 x, u8 y, char c);
void LCD_SHOW_String(u8 x, u8 y, u8 len, const char* str);
void LCD_SHOW_16HZ(u8 x, u8 y, const u8* cn_map);
void LCD_SHOW_Num(u8 x, u8 y, u32 num, u8 len, u8 mode);
void LCD_SHOW_Image(u8* p);

#endif
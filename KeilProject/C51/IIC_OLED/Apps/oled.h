#ifndef __IIC_OLED_H__
#define __IIC_OLED_H__

#include "util.h"

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// OLED 初始化
void OLED_init();
// 清屏
void OLED_clear();
// 显示图片
void OLED_showImage(const u8* imageMap);
// 显示中文字符串
void OLED_showChinese(u8 x, u8 y, u8 font_w, const char* str);
// 显示英文字符串
void OLED_showString(u8 x, u8 y, const char* str);

#endif

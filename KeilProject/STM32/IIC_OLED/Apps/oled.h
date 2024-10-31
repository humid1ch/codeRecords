#ifndef __OLED_H__
#define __OLED_H__

#include "stm32f10x.h"
#include "utils.h"

#define OLED_SCL_PORT GPIOB
#define OLED_SCL_PIN GPIO_Pin_6
#define OLED_SCL_RCC RCC_APB2Periph_GPIOB
#define OLED_SCL PBout(6)

#define OLED_SDA_PORT GPIOB
#define OLED_SDA_PIN GPIO_Pin_7
#define OLED_SDA_RCC RCC_APB2Periph_GPIOB
#define OLED_SDA PBout(7)
#define OLED_SDA_R PBin(7) // OLED 用不到

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// OLED 初始化
void OLED_init(void);
// 清屏
void OLED_clear(void);
// 显示图片
void OLED_showImage(const u8* imageMap);
// 显示中文字符串
void OLED_showChinese(u8 x, u8 y, u8 font_w, const char* str);
// 显示英文字符串
void OLED_showString(u8 x, u8 y, const char* str);

#endif

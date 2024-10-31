#ifndef __LCD9648_H__
#define __LCD9648_H__

#include "stm32f10x.h"
#include "utils.h"

#define LCD9648_RST_RCC RCC_APB2Periph_GPIOA
#define LCD9648_RS_RCC RCC_APB2Periph_GPIOA
#define LCD9648_CS0_RCC RCC_APB2Periph_GPIOA
#define LCD9648_SDA_RCC RCC_APB2Periph_GPIOA
#define LCD9648_SCL_RCC RCC_APB2Periph_GPIOA

#define LCD9648_RST_PORT GPIOA
#define LCD9648_RS_PORT GPIOA
#define LCD9648_CS0_PORT GPIOA
#define LCD9648_SDA_PORT GPIOA
#define LCD9648_SCL_PORT GPIOA

#define LCD9648_RST_PIN GPIO_Pin_3
#define LCD9648_CS0_PIN GPIO_Pin_4
#define LCD9648_RS_PIN GPIO_Pin_5
#define LCD9648_SDA_PIN GPIO_Pin_6
#define LCD9648_SCL_PIN GPIO_Pin_7

#define LCD9648_RST PAout(3)
#define LCD9648_CS0 PAout(4)
#define LCD9648_RS PAout(5)
#define LCD9648_SDA PAout(6)
#define LCD9648_SCL PAout(7)

//LCD显示范围
#define LCD_WIDTH 96
#define LCD_HEIGHT 48

//函数声明
void LCD9648_init(void);
void LCD9648_clear(void);
void LCD9648_showChinese(u8 x, u8 y, u8 font_wh, const char* str);
void LCD9648_showString(u8 x, u8 y, const char* str);
void LCD9648_show16HZ(u8 x, u8 y, const u8* cn_map);
void LCD9648_showNum(u8 x, u8 y, u32 num, u8 len, u8 mode);
void LCD9648_showImage(u8* p);

#endif

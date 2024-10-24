#ifndef __ED_DOTS_H__
#define __ED_DOTS_H__

#include "stm32f10x.h"

// 位带操作, 实现 51 类似的 GPIO 控制功能
// IO 口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long*)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
// 0x40010C0C
#define GPIOB_ODR_Addr (GPIOB_BASE + 12)
// 输出
#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr, n)

#define SCLK PBout(3)
#define SER PBout(4)
#define RCLK PBout(5)

#define SCLK_PORT GPIOB
#define SCLK_PIN GPIO_Pin_3
#define SCLK_PORT_RCC RCC_APB2Periph_GPIOB

#define SER_PORT GPIOB
#define SER_PIN GPIO_Pin_4
#define SER_PORT_RCC RCC_APB2Periph_GPIOB

#define RCLK_PORT GPIOB
#define RCLK_PIN GPIO_Pin_5
#define RCLK_PORT_RCC RCC_APB2Periph_GPIOB

#define COLS_PORT GPIOA
#define COLS_PIN GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7
#define COLS_PORT_RCC RCC_APB2Periph_GPIOA

#define WRITE_H8 1
#define WRITE_L8 0

#define SCLK_UP_ONCE()                                                                                                                                                                                 \
    {                                                                                                                                                                                                  \
        SCLK = 0;                                                                                                                                                                                      \
        delayUs(10);                                                                                                                                                                                   \
        SCLK = 1;                                                                                                                                                                                      \
        delayUs(10);                                                                                                                                                                                   \
        SCLK = 0;                                                                                                                                                                                      \
    }

#define RCLK_UP_ONCE()                                                                                                                                                                                 \
    {                                                                                                                                                                                                  \
        RCLK = 0;                                                                                                                                                                                      \
        delayUs(10);                                                                                                                                                                                   \
        RCLK = 1;                                                                                                                                                                                      \
        delayUs(10);                                                                                                                                                                                   \
        RCLK = 0;                                                                                                                                                                                      \
    }

void LEDOTS_init(void);
void LEDOTS_writeByte(GPIO_TypeDef* GPIOx, u8 value, u8 HL8Bits);
void HC595SendByte(u8 u8data);
void LEDOTS_rowCirculate(void);
void LEDOTS_colCirculate(void);
void LEDOTS_leftScroll(const u8* array, const u8 size);

#endif

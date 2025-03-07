#ifndef __SMG_H__
#define __SMG_H__

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

// SMG 位选
#define SMG_BIT2 PBout(3)
#define SMG_BIT1 PBout(4)
#define SMG_BIT0 PBout(5)

// 位选 GPIO初始化宏
#define SMG_BIT_RCC RCC_APB2Periph_GPIOB
#define SMG_BIT_PORT GPIOB
#define SMG_BIT0_PIN GPIO_Pin_3
#define SMG_BIT1_PIN GPIO_Pin_4
#define SMG_BIT2_PIN GPIO_Pin_5
// 段选 GPIO初始化宏
#define SMG_SEG_RCC RCC_APB2Periph_GPIOA
#define SMG_SEG_PORT GPIOA
#define SMG_SEG0_PIN GPIO_Pin_0
#define SMG_SEG1_PIN GPIO_Pin_1
#define SMG_SEG2_PIN GPIO_Pin_2
#define SMG_SEG3_PIN GPIO_Pin_3
#define SMG_SEG4_PIN GPIO_Pin_4
#define SMG_SEG5_PIN GPIO_Pin_5
#define SMG_SEG6_PIN GPIO_Pin_6
#define SMG_SEG7_PIN GPIO_Pin_7

#define WRITE_H8 1
#define WRITE_L8 0

// 位选函数指针
typedef void (*smgSelFunc)();
// SMG位选函数指针数组
extern smgSelFunc smgFuncs[];
// 段选数组
extern unsigned char smgNum[];

void SMG_init(void);
void SMG_writeByte(GPIO_TypeDef* GPIOx, u8 value, u8 HL8Bits);
void SMG_display(const u8* smgCode);

#endif

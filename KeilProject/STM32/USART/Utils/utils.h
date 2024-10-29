#ifndef __UTILS_H__
#define __UTILS_H__

#include "stm32f10x.h"

// 位带操作, 实现 51 类似的 GPIO 控制功能
// IO 口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long*)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOA_ODR_Addr (GPIOA_BASE + 12)
#define GPIOA_IDR_Addr (GPIOA_BASE + 8)

#define GPIOB_ODR_Addr (GPIOB_BASE + 12)
#define GPIOB_IDR_Addr (GPIOB_BASE + 8)

#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr, n)
#define PBin(n) BIT_ADDR(GPIOB_IDR_Addr, n)

#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr, n)
#define PAin(n) BIT_ADDR(GPIOA_IDR_Addr, n)

void SysTick_init(u8 SYSCLK);
void delayUs(u32 nUs);
void delayMs(u16 nMs); // nMs最大1860

#endif

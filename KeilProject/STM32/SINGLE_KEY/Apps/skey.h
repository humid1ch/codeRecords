#ifndef __SKEY_H__
#define __SKEY_H__

#include "stm32f10x.h"

// 位带操作, 实现 51 类似的 GPIO 控制功能
// IO 口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long*)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
// 0x40010C0C
#define GPIOA_IDR_Addr (GPIOA_BASE + 8)
// 输出
#define PAin(n) BIT_ADDR(GPIOA_IDR_Addr, n)

#define S_KEY1 PAin(15)
#define S_KEY2 PAin(14)
#define S_KEY3 PAin(13)
#define S_KEY4 PAin(12)

#define S_KEY_RCC RCC_APB2Periph_GPIOA
#define S_KEY_PORT GPIOA
#define S_KEY1_PIN GPIO_Pin_15
#define S_KEY2_PIN GPIO_Pin_14
#define S_KEY3_PIN GPIO_Pin_13
#define S_KEY4_PIN GPIO_Pin_12

#define NON_KEY_PRESSED 0x00
#define S_KEY1_PRESSED 0x08
#define S_KEY2_PRESSED 0x04
#define S_KEY3_PRESSED 0x02
#define S_KEY4_PRESSED 0x01

void SKEY_init(void);
u8 SKEY_keyScan(void);

#endif

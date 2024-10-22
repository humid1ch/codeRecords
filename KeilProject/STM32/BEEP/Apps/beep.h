#ifndef __BEEP_H__
#define __BEEP_H__

#include "stm32f10x.h"

// 位带操作,实现 51 类似的 GPIO 控制功能
// IO 口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long*)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
// 0x40010C0C
#define GPIOB_ODR_Addr (GPIOB_BASE + 12)
// 输出
#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr, n)

#define BEEP PBout(0)

#define BEEP_PORT_RCC RCC_APB2Periph_GPIOB
#define BEEP_PORT GPIOB
#define BEEP_PIN GPIO_Pin_0

// 脉冲频率
#define FEQUENCY_S 2000

void BEEP_init(void);
void BEEP_on(u8 seconds);

#endif

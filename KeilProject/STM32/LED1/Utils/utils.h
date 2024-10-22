#ifndef __UTILS_H__
#define __UTILS_H__

#include "stm32f10x.h"

void SysTick_Init(u8 SYSCLK);
void delayUs(u32 nUs);
void delayMs(u16 nMs); // nMs最大1860

#endif

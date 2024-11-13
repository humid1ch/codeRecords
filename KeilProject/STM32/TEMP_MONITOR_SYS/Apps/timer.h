#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f10x.h"
#include "utils.h"

// 参数:
// TIMx: 选取定时器
// period: 重载计数值
// prescaler: 预分频系数
void TIMER_init(TIM_TypeDef* TIMx, u16 period, u16 prescaler);

#endif

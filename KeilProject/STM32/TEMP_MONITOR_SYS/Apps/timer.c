#include "timer.h"

void TIMER_init(TIM_TypeDef* TIMx, u16 period, u16 prescaler) {
    NVIC_InitTypeDef NVIC_InitStructure;
    if (TIMx == TIM2) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    }
    if (TIMx == TIM3) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    }
    if (TIMx == TIM4) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    }
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructuce;
    TIM_TimeBaseInitStructuce.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分频
    TIM_TimeBaseInitStructuce.TIM_CounterMode = TIM_CounterMode_Up; // 计数模式
    TIM_TimeBaseInitStructuce.TIM_Period = period;                  // 定时器周期
    TIM_TimeBaseInitStructuce.TIM_Prescaler = prescaler;            // 定时器预分频器
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStructuce);
    TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE); // 开启定时器中断

    TIM_Cmd(TIMx, ENABLE); // 开启定时器
}

#include "utils.h"

#define SYSTICK_ENABLE 1
#define SYSTICK_DISABLE 0

static u8 facUs = 0;  //us延时的倍数
static u16 facMs = 0; //ms延时的倍数

//SYSCLK表示系统时钟频率，单位MHz
void SysTick_Init(u8 SYSCLK) {
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); // 确定系统定时器的工作频率
    facUs = SYSCLK / 8;
    facMs = facUs * 1000;
}

void delayUs(u32 nUs) {
    SysTick->LOAD = nUs * facUs; // 设系统定时器的预值 以及 重载值
    SysTick->VAL = 0;

    SysTick->CTRL |= SYSTICK_ENABLE; // 开始倒计数

    u32 u32temp = 0;

    do {
        u32temp = SysTick->CTRL;
    } while ((u32temp & 0x01) && !(u32temp & (1 << 16))); // 等待时间到达, 同时保证 定时器开启

    SysTick->CTRL &= SYSTICK_DISABLE; // 关闭倒计数
    SysTick->VAL = 0;
}

// nMs的最大值不能超过1860
void delayMs(u16 nMs) {
    if (nMs > 1860)
        nMs = 1860;
    SysTick->LOAD = nMs * facMs; // 设系统定时器的预值 以及 重载值
    SysTick->VAL = 0;            // 写入, 会将计时器清零, 并且 CTRL的countFlag 也被清0

    SysTick->CTRL |= SYSTICK_ENABLE; // 开始倒计数

    u32 u32temp = 0;

    do {
        u32temp = SysTick->CTRL;
    } while ((u32temp & 0x01) && !(u32temp & (1 << 16))); // 等待时间到达, 同时保证 定时器开启

    SysTick->CTRL &= SYSTICK_DISABLE; // 关闭倒计数
    SysTick->VAL = 0;
}

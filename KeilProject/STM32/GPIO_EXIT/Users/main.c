#include "stm32f10x_it.h"
#include "utils.h"
#include "my_exti.h"
#include "skey.h"
#include "led.h"

int main() {
    SysTick_init(72);
    // 中断优先级组选择, 第2组(0~4)
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    SKEY_init();
    MY_EXTI_init();
    LED_init();

    while (1) {
    }
}

// 15_10线路的中断函数, 需要与官方提供的接口名保持一致
void EXTI15_10_IRQHandler(void) {
    // 在初始化中, 设置了 外部中断 上升沿触发
    if (EXTI_GetITStatus(EXTI_Line12))
        LED3 = ~LED3;
    if (EXTI_GetITStatus(EXTI_Line13))
        LED2 = ~LED2;
    if (EXTI_GetITStatus(EXTI_Line14))
        LED0 = ~LED0;
    if (EXTI_GetITStatus(EXTI_Line15))
        LED0 = ~LED0;

    // 中断处理之后, 清除外部中断对应线路的中断触发标识
    EXTI_ClearITPendingBit(EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15);
}

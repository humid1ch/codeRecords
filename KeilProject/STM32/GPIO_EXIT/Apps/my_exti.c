#include "my_exti.h"
#include "utils.h"

void MY_EXTI_init(void) {
    // 中断引脚 映射等配置 初始化结构体
    EXTI_InitTypeDef EXTI_initStruct;
    // 中断控制器 初始化结构体
    NVIC_InitTypeDef NVIC_initStruct;

    // 开启外部中断时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // GPIO 映射外部中断(独立按键)
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource13);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource14);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);

    // 中断控制器模块 初始化
    // 线路选择
    NVIC_initStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    // 使能
    NVIC_initStruct.NVIC_IRQChannelCmd = ENABLE;
    // 优先级上, 选择 第2组
    // 抢占优先级(父)
    NVIC_initStruct.NVIC_IRQChannelPreemptionPriority = 2;
    // 响应优先级(子)
    NVIC_initStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_initStruct);

    // 外部中断模块初始化
    // GPIO引脚标号 与 EXTI线路标号需要保持一致的, 是这样设计的
    EXTI_initStruct.EXTI_Line = EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15;
    // 中断 对应线路使能
    EXTI_initStruct.EXTI_LineCmd = ENABLE;
    // EXIT 设置 为外部中断模式
    EXTI_initStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    // 设置中断触发方式, 上升沿 还是 下降沿触发 (上升沿)
    EXTI_initStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_initStruct);
}

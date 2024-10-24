#ifndef __STEP_MOTOR_H__
#define __STEP_MOTOR_H__

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

#define MOTO_D PBout(8)
#define MOTO_C PBout(9)
#define MOTO_B PBout(12)
#define MOTO_A PBout(13)

#define MOTO_INS_PORT_RCC RCC_APB2Periph_GPIOB
#define MOTO_INS_PORT GPIOB
#define MOTO_D_PIN GPIO_Pin_8
#define MOTO_C_PIN GPIO_Pin_9
#define MOTO_B_PIN GPIO_Pin_12
#define MOTO_A_PIN GPIO_Pin_13

// 顺时针
#define CLOCK_WISE 1
// 逆时针
#define COUNTER_CLOCK_WISE 0
// 启动
#define MOTOR_RUN 1
// 停止
#define MOTOR_STOP 0
// 转速 高速
#define HIGH_SPEED 2
// 低速
#define LOW_SPEED 1

// 步进电机初始化
void STEP_MOTOR_init(void);

void STEP_MOTOR_run(u8 states, u8 clock, u8 speed, u16 angle);

#endif

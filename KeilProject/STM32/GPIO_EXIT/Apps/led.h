#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h"
#include "utils.h"

//位带操作,实现 51 类似的 GPIO 控制功能
//IO 口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))

#define MEM_ADDR(addr) *((volatile unsigned long*)(addr))

#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
// 0x40010C0C
#define GPIOA_ODR_Addr (GPIOA_BASE + 12)
// 输出
#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr, n)

#define LED0 PAout(0)
#define LED1 PAout(1)
#define LED2 PAout(2)
#define LED3 PAout(3)

#define LEDS_PORT GPIOA
#define LEDS_PORT_RCC RCC_APB2Periph_GPIOA
#define LED0_PIN GPIO_Pin_0
#define LED1_PIN GPIO_Pin_1
#define LED2_PIN GPIO_Pin_2
#define LED3_PIN GPIO_Pin_3
#define LED4_PIN GPIO_Pin_4
#define LED5_PIN GPIO_Pin_5
#define LED6_PIN GPIO_Pin_6
#define LED7_PIN GPIO_Pin_7

#define WRITE_H8 1
#define WRITE_L8 0

void LED_init(void);

void LED_writeByte(GPIO_TypeDef* GPIOx, u8 value, u8 HL8Bits);

#endif

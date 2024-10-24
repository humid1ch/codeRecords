#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h"
#include "utils.h"

//位带操作,实现 51 类似的 GPIO 控制功能
//IO 口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))

#define MEM_ADDR(addr) *((volatile unsigned long*)(addr))

#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))

#define LED0_PORT GPIOA
#define LED0_PIN GPIO_Pin_0
#define LED0_PORT_RCC RCC_APB2Periph_GPIOA

#define LED1_PORT GPIOA
#define LED1_PIN GPIO_Pin_1
#define LED1_PORT_RCC RCC_APB2Periph_GPIOA

#define LED2_PORT GPIOA
#define LED2_PIN GPIO_Pin_2
#define LED2_PORT_RCC RCC_APB2Periph_GPIOA

#define LED3_PORT GPIOA
#define LED3_PIN GPIO_Pin_3
#define LED3_PORT_RCC RCC_APB2Periph_GPIOA

#define LED4_PORT GPIOA
#define LED4_PIN GPIO_Pin_4
#define LED4_PORT_RCC RCC_APB2Periph_GPIOA

#define LED5_PORT GPIOA
#define LED5_PIN GPIO_Pin_5
#define LED5_PORT_RCC RCC_APB2Periph_GPIOA

#define LED6_PORT GPIOA
#define LED6_PIN GPIO_Pin_6
#define LED6_PORT_RCC RCC_APB2Periph_GPIOA

#define LED7_PORT GPIOA
#define LED7_PIN GPIO_Pin_7
#define LED7_PORT_RCC RCC_APB2Periph_GPIOA

#define WRITE_H8 1
#define WRITE_L8 0

void LED_init(void);

void LED_writeByte(GPIO_TypeDef* GPIOx, u8 value, u8 HL8Bits);

#endif

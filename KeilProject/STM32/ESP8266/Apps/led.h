#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h"
#include "utils.h"

#define LEDS_PORT GPIOA
#define LEDS_PORT_RCC RCC_APB2Periph_GPIOA

#define LED1_PIN GPIO_Pin_0
#define LED2_PIN GPIO_Pin_1
#define LED3_PIN GPIO_Pin_2
#define LED4_PIN GPIO_Pin_3
#define LED5_PIN GPIO_Pin_4
#define LED6_PIN GPIO_Pin_5
#define LED7_PIN GPIO_Pin_6
#define LED8_PIN GPIO_Pin_7

#define WRITE_H8 1
#define WRITE_L8 0

void LED_init(void);

void LED_writeByte(GPIO_TypeDef* GPIOx, u8 value, u8 HL8Bits);

#endif

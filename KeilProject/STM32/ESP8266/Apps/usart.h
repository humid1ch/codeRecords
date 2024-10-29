#ifndef __UASRT_H__
#define __UASRT_H__

#include "stm32f10x.h"
#include <stdio.h>

#define USART1_PORT GPIOA
#define USART1_TX_PIN GPIO_Pin_9
#define USART1_RX_PIN GPIO_Pin_10
#define USART1_PORT_RCC RCC_APB2Periph_GPIOA
#define USART1_RCC RCC_APB2Periph_USART1

#define USART2_PORT GPIOA
#define USART2_TX_PIN GPIO_Pin_2
#define USART2_RX_PIN GPIO_Pin_3
#define USART2_PORT_RCC RCC_APB2Periph_GPIOA

#define USART2_RCC RCC_APB1Periph_USART2

#define USART3_PORT GPIOB
#define USART3_TX_PIN GPIO_Pin_10
#define USART3_RX_PIN GPIO_Pin_11
#define USART3_PORT_RCC RCC_APB2Periph_GPIOB

#define USART3_RCC RCC_APB1Periph_USART3

#define USART1_USED 1
#define USART2_USED 2
#define USART3_USED 3

extern u8 USART_use;

#define USART1_USE() USART_use = USART1_USED
#define USART2_USE() USART_use = USART2_USED
#define USART3_USE() USART_use = USART3_USED

void USART1_init(u32 u32baudRate);
void USART2_init(u32 u32baudRate);
void USART3_init(u32 u32baudRate);

#endif

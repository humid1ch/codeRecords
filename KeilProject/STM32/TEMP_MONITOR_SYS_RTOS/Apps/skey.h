#ifndef __SKEY_H__
#define __SKEY_H__

#include "stm32f10x.h"
#include "utils.h"

#define S_KEY1 PAin(15)
#define S_KEY2 PAin(14)
#define S_KEY3 PAin(13)
#define S_KEY4 PAin(12)

#define S_KEY_RCC RCC_APB2Periph_GPIOA
#define S_KEY_PORT GPIOA
#define S_KEY1_PIN GPIO_Pin_15
#define S_KEY2_PIN GPIO_Pin_14
#define S_KEY3_PIN GPIO_Pin_13
#define S_KEY4_PIN GPIO_Pin_12

#define NON_KEY_PRESSED 0x00
#define S_KEY1_PRESSED 0x08
#define S_KEY2_PRESSED 0x04
#define S_KEY3_PRESSED 0x02
#define S_KEY4_PRESSED 0x01

void SKEY_init(void);
u8 SKEY_keyScan(void);

#endif

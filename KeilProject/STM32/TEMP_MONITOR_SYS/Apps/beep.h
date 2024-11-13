#ifndef __BEEP_H__
#define __BEEP_H__

#include "stm32f10x.h"
#include "utils.h"

#define BEEP PBout(0)

#define BEEP_PORT_RCC RCC_APB2Periph_GPIOB
#define BEEP_PORT GPIOB
#define BEEP_PIN GPIO_Pin_0

// 脉冲频率
#define FEQUENCY_S 2000

void BEEP_init(void);
void BEEP_on(u8 seconds);
void BEEP_pulse50Us(void);

#endif

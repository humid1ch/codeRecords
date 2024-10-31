#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "stm32f10x.h"
#include "utils.h"

#define DS18B20_DQ_PORT GPIOA
#define DS18B20_DQ_RCC RCC_APB2Periph_GPIOA
#define DS18B20_DQ_PIN GPIO_Pin_11

#define DS18B20_DQ PAout(11)
#define DS18B20_DQ_R PAin(11)

void DS18B20_init(void);
void DS18B20_readTemperature(u16* u16pdata);

#endif

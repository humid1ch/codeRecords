#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "stm32f10x.h"
#include "utils.h"

#define ESP_RST_PORT GPIOB
#define ESP_RST_PIN GPIO_Pin_2
#define ESP_RST_RCC RCC_APB2Periph_GPIOB

#define ESP_CH_PD_PORT GPIOA
#define ESP_CH_PD_PIN GPIO_Pin_15
#define ESP_CH_PD_RCC RCC_APB2Periph_GPIOA

#define ESP_RST PBout(2)
#define ESP_CH_PD PAout(15)

void ESP8266_init(void);
void ESP8266_sendATCmd(const char* cmd);

#endif

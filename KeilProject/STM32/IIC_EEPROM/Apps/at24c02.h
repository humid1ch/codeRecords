#ifndef __AT24C02_H__
#define __AT24C02_H__

#include "stm32f10x.h"
#include "utils.h"

//定义EEPROM的通信引脚
#define EEPROM_SCL_PORT GPIOB               // 定义GPIO的组
#define EEPROM_SCL_PIN GPIO_Pin_6           // 定义GPIO的位
#define EEPROM_SCL_RCC RCC_APB2Periph_GPIOB // 定义GPIO的时钟总线
#define EEPROM_SCL PBout(6)

#define EEPROM_SDA_PORT GPIOB               // 定义GPIO的组
#define EEPROM_SDA_PIN GPIO_Pin_7           // 定义GPIO的位
#define EEPROM_SDA_RCC RCC_APB2Periph_GPIOB // 定义GPIO的时钟总线
#define EEPROM_SDA PBout(7)
#define EEPROM_SDA_R PBin(7)

#define AT24C02_ADDR 0xA0      // AT24C02的地址
#define AT24C02_PAGE_SIZE 32   // AT24C02的页大小
#define AT24C02_PAGE_NUM 8     // AT24C02的页数量
#define AT24C02_TOTAL_SIZE 256 // AT24C02的总大小

#define AT24C02_CMD_READ (AT24C02_ADDR | 1)  // 读命令
#define AT24C02_CMD_WRITE (AT24C02_ADDR | 0) // 写命令

void AT24C02_init(void);
u8 AT24c02_writeByte(u8 addr, u8 u8data);  // 写一个字节，成功返回0
u8 AT24c02_weadByte(u8 addr, u8* u8pdata); // 读一个字节，成功返回0

#endif

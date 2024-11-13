#ifndef __ONEWRITE_H__
#define __ONEWRITE_H__

#include "stm32f10x.h"
#include "utils.h"
#include "ds18b20.h"

#define DATA_IO_PORT DS18B20_DQ_PORT
#define DATA_IO_RCC DS18B20_DQ_RCC
#define DATA_IO_PIN DS18B20_DQ_PIN

#define DATA_IO DS18B20_DQ
#define DATA_IO_R DS18B20_DQ_R

void OW_init(void);           //one-write初始化
void OW_writeByte(u8 u8data); //one-write写一个字节数据
u8 OW_readByte(void);         //one-write读一个字节数据
u8 OW_reset(void);            //one-write设备复位

#endif

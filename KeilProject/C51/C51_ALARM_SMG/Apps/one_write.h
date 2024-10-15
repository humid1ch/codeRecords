#ifndef __ONEWRITE_H__
#define __ONEWRITE_H__

#include "util.h"

void ONE_WRITE_Init(); //one-write初始化

void ONE_WRITE_WriteByte(u8 u8data); //one-write写一个字节数据
u8 ONE_WRITE_ReadByte();         //one-write读一个字节数据
u8 ONE_WRITE_Reset();            //one-write设备复位

#endif
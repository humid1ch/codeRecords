#ifndef __ONEWRITE_H__
#define __ONEWRITE_H__

#include "util.h"
#include "reg52.h"

void oneWriteInit(void); //one-write初始化

void oneWriteWriteByte(u8 u8data); //one-write写一个字节数据
u8 oneWriteReadByte();         //one-write读一个字节数据
u8 oneWriteReset();            //one-write设备复位

#endif
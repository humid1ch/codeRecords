#ifndef __IIC_H__
#define __IIC_H__

#include "util.h"

void iicStart();
void iicStop();
u8 iicWaitAck();
void iicAck();
void iicNAck();
void iicWriteByte(u8 u8data);
u8 iicReadByte();

#endif
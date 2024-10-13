#ifndef __IIC_H__
#define __IIC_H__

#include "util.h"

void IIC_Start();
void IIC_Stop();
u8 IIC_WaitAck();
void IIC_Ack();
void IIC_NAck();
void IIC_WriteByte(u8 u8data);
u8 IIC_ReadByte();

#endif
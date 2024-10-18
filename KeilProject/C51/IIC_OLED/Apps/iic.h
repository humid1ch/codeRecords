#ifndef __IIC_H__
#define __IIC_H__

#include "util.h"

void IIC_start();
void IIC_stop();
u8 IIC_waitAck();
void IIC_ack();
void IIC_nAck();
void IIC_writeByte(u8 u8data);
u8 IIC_readByte();

#endif

#ifndef __IIC_H__
#define __IIC_H__

#include "stm32f10x.h"

// 初始化 IIC 总线
void IIC_init(void);

//IIC 开始信号
void IIC_start(void);

//IIC 停止信号
void IIC_stop(void);

//IIC 等待应答信号
//返回值：(ack)1，接收应答；0(nack)，未接收应答
u8 IIC_waitAck(void);

//IIC 发送一个字节
void IIC_writeByte(u8 u8data);

//IIC 读取一个字节
u8 IIC_readByte(void);

//IIC 发送应答ACK
void IIC_sendAck(void);

//IIC 发送非应答NACK
void IIC_sendNack(void);

#endif

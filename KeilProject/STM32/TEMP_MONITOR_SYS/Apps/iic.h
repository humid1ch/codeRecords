#ifndef __IIC_H__
#define __IIC_H__

#include "stm32f10x.h"
#include "oled.h"

#define IIC_SCL_PORT OLED_SCL_PORT
#define IIC_SCL_PIN OLED_SCL_PIN
#define IIC_SCL_RCC OLED_SCL_RCC
#define IIC_SCL OLED_SCL

#define IIC_SDA_PORT OLED_SDA_PORT
#define IIC_SDA_PIN OLED_SDA_PIN
#define IIC_SDA_RCC OLED_SDA_RCC
#define IIC_SDA OLED_SDA
#define IIC_SDA_R OLED_SDA_R

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

// 硬 IIC 初始化
void IIC_initHard(void);
// 硬 IIC 写
void IIC_writeReg(u8 regAddress, u8 u8data);
// 硬 IIC 读
u8 IIC_readReg(u8 regAddress);

#endif

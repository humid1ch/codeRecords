#ifndef __UART_H__
#define __UART_H__

#include "util.h"
#include "stdio.h"

#define UART_RX_BUF_LENTH 20
#define RECV_N 0x8000
#define RECV_R 0x4000

extern u8 uartBuf[]; //串口接收缓冲池，最大20个字节

//发送方在一串字符结尾有两个特殊字符'\r','\n'(0x0d,0x0a)

extern u16 uartRxStatus; //串口接收状态标志
//bit15			0:表示没接收完成，			1：表示字符接收完成
//bit14			0:表示没接收到'\r'(0x0d);	1：表示收到了'\r'
//bit13~0		表示接收到的数据长度

void uartInit(void);
void uartSendData(u8 u8data);
void uartRecvData(void);

#endif
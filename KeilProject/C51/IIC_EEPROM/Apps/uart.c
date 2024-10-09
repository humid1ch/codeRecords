#include "reg52.h"
#include "uart.h"

u8 uartBuf[UART_RX_BUF_LENTH] = {0}; //串口接收缓冲池，最大30个字节
u16 uartRxStatus = 0;                //串口接收状态标志

void uartInit(void) {
    //串口模块寄存器
    PCON |= 0x80; //使能波特率倍速位SMOD
    SCON = 0x50;  //8位数据,可变波特率

    //定时器1相关寄存器
    TMOD &= 0x0F; //设置定时器模式
    TMOD |= 0x20; //设置定时器工作方式2，8位自重载
    TL1 = 0xFA;   //设置定时初始值
    TH1 = 0xFA;   //设置定时重载值
    TR1 = 1;      //定时器1开始计时

    //中断相关寄存器
    ES = 1; //使能串口1中断
    EA = 1; //使能中断总开关
}

void uartSendData(u8 u8data) {
    SBUF = u8data;
    while (!TI)
        ;
    TI = 0;
}

void uartRecvData(void) {
    u8 u8RecData = 0;
    u8RecData = SBUF;

    //接收没有完成
    if ((uartRxStatus & RECV_N) == 0) {
        //已经接收到了'\r'
        if (uartRxStatus & RECV_R) {
            if (u8RecData != '\n')
                uartRxStatus = 0; //将接收状态清0
            else 
                uartRxStatus |= RECV_N; //数据接收完成
        }
        else {
            //还没接收到'\r'
            if (u8RecData == '\r')
                uartRxStatus |= RECV_R; //表示收到了'\r'
            else {
                //是有效数据，需要放入用户数组
                if ((uartRxStatus & 0x3fff) < UART_RX_BUF_LENTH) {
                    uartBuf[uartRxStatus & 0x3fff] = u8RecData;
                    uartRxStatus++;
                }
                else
                    uartRxStatus = 0;
            }
        }
    }
}

char putchar(char dat) {
    uartSendData(dat);

    return dat;
}

void uart1Isr(void) interrupt 4 {
    //检测串口1接收中断
    if (RI) {
        RI = 0; //清除串口1接收中断请求位
        uartRecvData();
    }
}
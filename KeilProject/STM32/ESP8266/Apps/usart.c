#include "usart.h"
#include "utils.h"

// 串口占用. 用于printf 输出重定向到串口
u8 USART_use = 0;

void USART1_init(u32 u32baudRate) {
    // 波特率 矫正
    if (u32baudRate > 9600)
        u32baudRate = 115200;
    if (u32baudRate < 9600)
        u32baudRate = 9600;

    RCC_APB2PeriphClockCmd(USART1_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(USART1_PORT_RCC, ENABLE);

    GPIO_InitTypeDef GPIO_txInitStructure;
    GPIO_txInitStructure.GPIO_Pin = USART1_TX_PIN;    // 发送引脚
    GPIO_txInitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 符合推挽模式
    GPIO_txInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART1_PORT, &GPIO_txInitStructure);

    GPIO_InitTypeDef GPIO_rxInitStructure;
    GPIO_rxInitStructure.GPIO_Pin = USART1_RX_PIN;          // 接收引脚
    GPIO_rxInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入模式
    GPIO_rxInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART1_PORT, &GPIO_rxInitStructure);

    USART_InitTypeDef USART_initStructure;
    USART_initStructure.USART_BaudRate = u32baudRate;                               // 波特率设置
    USART_initStructure.USART_WordLength = USART_WordLength_8b;                     // 数据长度
    USART_initStructure.USART_StopBits = USART_StopBits_1;                          // 停止位
    USART_initStructure.USART_Parity = USART_Parity_No;                             //
    USART_initStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //
    USART_initStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 // 发送 和 接收模式
    USART_Init(USART1, &USART_initStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_InitTypeDef NVIC_initStructure;
    NVIC_initStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_initStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_initStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_initStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_initStructure);

    USART_Cmd(USART1, ENABLE);
}

void USART2_init(u32 u32baudRate) {
    // 波特率 矫正
    if (u32baudRate > 9600)
        u32baudRate = 115200;
    if (u32baudRate < 9600)
        u32baudRate = 9600;

    RCC_APB1PeriphClockCmd(USART2_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(USART2_PORT_RCC, ENABLE);

    GPIO_InitTypeDef GPIO_txInitStructure;
    GPIO_txInitStructure.GPIO_Pin = USART2_TX_PIN;    // 发送引脚
    GPIO_txInitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽模式
    GPIO_txInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART2_PORT, &GPIO_txInitStructure);

    GPIO_InitTypeDef GPIO_rxInitStructure;
    GPIO_rxInitStructure.GPIO_Pin = USART2_RX_PIN;          // 接收引脚
    GPIO_rxInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入模式
    GPIO_rxInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART2_PORT, &GPIO_rxInitStructure);

    USART_InitTypeDef USART_initStructure;
    USART_initStructure.USART_BaudRate = u32baudRate;                               // 波特率设置
    USART_initStructure.USART_WordLength = USART_WordLength_8b;                     // 数据长度
    USART_initStructure.USART_StopBits = USART_StopBits_1;                          // 停止位
    USART_initStructure.USART_Parity = USART_Parity_No;                             //
    USART_initStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //
    USART_initStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 // 发送 和 接收模式
    USART_Init(USART2, &USART_initStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    NVIC_InitTypeDef NVIC_initStructure;
    NVIC_initStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_initStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_initStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_initStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_initStructure);

    USART_Cmd(USART2, ENABLE); // 串口使能
}

void USART3_init(u32 u32baudRate) {
    // 波特率 矫正
    if (u32baudRate > 9600)
        u32baudRate = 115200;
    if (u32baudRate < 9600)
        u32baudRate = 9600;

    RCC_APB1PeriphClockCmd(USART3_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(USART3_PORT_RCC, ENABLE);

    GPIO_InitTypeDef GPIO_txInitStructure;
    GPIO_txInitStructure.GPIO_Pin = USART3_TX_PIN;    // 发送引脚
    GPIO_txInitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽模式
    GPIO_txInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART3_PORT, &GPIO_txInitStructure);

    GPIO_InitTypeDef GPIO_rxInitStructure;
    GPIO_rxInitStructure.GPIO_Pin = USART3_RX_PIN;          // 接收引脚
    GPIO_rxInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入模式
    GPIO_rxInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART3_PORT, &GPIO_rxInitStructure);

    USART_InitTypeDef USART_initStructure;
    USART_initStructure.USART_BaudRate = u32baudRate;                               // 波特率设置
    USART_initStructure.USART_WordLength = USART_WordLength_8b;                     // 数据长度
    USART_initStructure.USART_StopBits = USART_StopBits_1;                          // 停止位
    USART_initStructure.USART_Parity = USART_Parity_No;                             //
    USART_initStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //
    USART_initStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 // 发送 和 接收模式
    USART_Init(USART3, &USART_initStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    NVIC_InitTypeDef NVIC_initStructure;
    NVIC_initStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_initStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_initStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_initStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_initStructure);

    USART_Cmd(USART3, ENABLE); // 串口使能
}

int fputc(int ch, FILE* p) {
    if (USART_use == USART1_USED) {
        USART_SendData(USART1, (u16)ch);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET)
            ;
    }

    if (USART_use == USART2_USED) {
        USART_SendData(USART2, (u16)ch);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET)
            ;
    }

    if (USART_use == USART3_USED) {
        USART_SendData(USART3, (u16)ch);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) != SET)
            ;
    }

    return ch;
}

#include "usart.h"
#include "utils.h"

void USART1_init(u32 u32baudRate) {
    // ������ ����
    if (u32baudRate > 9600)
        u32baudRate = 115200;
    if (u32baudRate < 9600)
        u32baudRate = 9600;

    RCC_APB2PeriphClockCmd(USART1_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(USART1_PORT_RCC, ENABLE);

    GPIO_InitTypeDef GPIO_txInitStructure;
    GPIO_txInitStructure.GPIO_Pin = USART1_TX_PIN;    // ��������
    GPIO_txInitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ��������ģʽ
    GPIO_txInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART1_PORT, &GPIO_txInitStructure);

    GPIO_InitTypeDef GPIO_rxInitStructure;
    GPIO_rxInitStructure.GPIO_Pin = USART1_RX_PIN;          // ��������
    GPIO_rxInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������ģʽ
    GPIO_rxInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART1_PORT, &GPIO_rxInitStructure);

    USART_InitTypeDef USART_initStructure;
    USART_initStructure.USART_BaudRate = u32baudRate;                               // ����������
    USART_initStructure.USART_WordLength = USART_WordLength_8b;                     // ���ݳ���
    USART_initStructure.USART_StopBits = USART_StopBits_1;                          // ֹͣλ
    USART_initStructure.USART_Parity = USART_Parity_No;                             //
    USART_initStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //
    USART_initStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 // ���� �� ����ģʽ
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
    // ������ ����
    if (u32baudRate > 9600)
        u32baudRate = 115200;
    if (u32baudRate < 9600)
        u32baudRate = 9600;

    RCC_APB1PeriphClockCmd(USART2_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(USART2_PORT_RCC, ENABLE);

    GPIO_InitTypeDef GPIO_txInitStructure;
    GPIO_txInitStructure.GPIO_Pin = USART2_TX_PIN;    // ��������
    GPIO_txInitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ��������ģʽ
    GPIO_txInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART2_PORT, &GPIO_txInitStructure);

    GPIO_InitTypeDef GPIO_rxInitStructure;
    GPIO_rxInitStructure.GPIO_Pin = USART2_RX_PIN;          // ��������
    GPIO_rxInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������ģʽ
    GPIO_rxInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART2_PORT, &GPIO_rxInitStructure);

    USART_InitTypeDef USART_initStructure;
    USART_initStructure.USART_BaudRate = u32baudRate;                               // ����������
    USART_initStructure.USART_WordLength = USART_WordLength_8b;                     // ���ݳ���
    USART_initStructure.USART_StopBits = USART_StopBits_1;                          // ֹͣλ
    USART_initStructure.USART_Parity = USART_Parity_No;                             //
    USART_initStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //
    USART_initStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 // ���� �� ����ģʽ
    USART_Init(USART2, &USART_initStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    NVIC_InitTypeDef NVIC_initStructure;
    NVIC_initStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_initStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_initStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_initStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_initStructure);

    USART_Cmd(USART2, ENABLE); // ����ʹ��
}

void USART3_init(u32 u32baudRate) {
    // ������ ����
    if (u32baudRate > 9600)
        u32baudRate = 115200;
    if (u32baudRate < 9600)
        u32baudRate = 9600;

    RCC_APB1PeriphClockCmd(USART3_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(USART3_PORT_RCC, ENABLE);

    GPIO_InitTypeDef GPIO_txInitStructure;
    GPIO_txInitStructure.GPIO_Pin = USART3_TX_PIN;    // ��������
    GPIO_txInitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ��������ģʽ
    GPIO_txInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART3_PORT, &GPIO_txInitStructure);

    GPIO_InitTypeDef GPIO_rxInitStructure;
    GPIO_rxInitStructure.GPIO_Pin = USART3_RX_PIN;          // ��������
    GPIO_rxInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������ģʽ
    GPIO_rxInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART3_PORT, &GPIO_rxInitStructure);

    USART_InitTypeDef USART_initStructure;
    USART_initStructure.USART_BaudRate = u32baudRate;                               // ����������
    USART_initStructure.USART_WordLength = USART_WordLength_8b;                     // ���ݳ���
    USART_initStructure.USART_StopBits = USART_StopBits_1;                          // ֹͣλ
    USART_initStructure.USART_Parity = USART_Parity_No;                             //
    USART_initStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //
    USART_initStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 // ���� �� ����ģʽ
    USART_Init(USART3, &USART_initStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    NVIC_InitTypeDef NVIC_initStructure;
    NVIC_initStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_initStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_initStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_initStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_initStructure);

    USART_Cmd(USART3, ENABLE); // ����ʹ��
}


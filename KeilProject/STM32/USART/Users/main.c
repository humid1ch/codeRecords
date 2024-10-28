#include "stm32f10x_it.h"
#include "utils.h"
#include "usart.h"

#define USART_REC_LEN 200 // 最大串口缓冲区

u8 USART1_RX_BUF[USART_REC_LEN]; // 串口1的接收缓冲
u16 USART1_RX_STA;               // 串口1接收状态标记+接收个数

u8 USART2_RX_BUF[USART_REC_LEN]; // 串口2的接收缓冲
u16 USART2_RX_STA;               // 串口2接收状态标记+接收个数

// 串口占用 标识
u8 USART_use = 1;

int main() {
    SysTick_init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 2级 优先级

    USART1_init(115200);
    USART2_init(115200);

    while (1) {
        if (USART1_RX_STA & 0x8000) {
            USART_use = 1; // 占用串口1
            printf("USART1 send: %s\r\n", USART1_RX_BUF);

            // 数据清零
            u16 dataLen = USART1_RX_STA & 0x3FFF;
            for (u16 i = 0; i < dataLen; i++)
                USART1_RX_BUF[i] = 0;
            USART1_RX_STA = 0;
        }

        if (USART2_RX_STA & 0x8000) {
            USART_use = 2; // 占用串口2
            printf("USART2 send: %s\r\n", USART2_RX_BUF);

            // 数据清零
            u16 dataLen = USART2_RX_STA & 0x3FFF;
            for (u16 i = 0; i < dataLen; i++)
                USART2_RX_BUF[i] = 0;
            USART2_RX_STA = 0;
        }
    }
}

void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
        u16 rec = USART_ReceiveData(USART1); // 从USART1模块的数据寄存器中读取接收到的数据

        // 判断接收是否完成, 即是否已经标记了 收到 '\n'
        if ((USART1_RX_STA & 0x8000) == 0) {
            // 判断有没有收到过 '\r'
            if (USART1_RX_STA & 0x4000) {
                // 数据接收不正常
                if (rec != 0x0A)
                    USART1_RX_STA = 0;
                else                         // 数据正常, 即收到 \n
                    USART1_RX_STA |= 0x8000; // 标记数据接收完成
            }
            else {
                // 表示接收到的是 \r
                if (rec == 0x0D)
                    USART1_RX_STA |= 0x4000; // 标记数据接收到\r
                else {
                    // 无论是\r 还是 \n, 都进行存储
                    USART1_RX_BUF[USART1_RX_STA & 0x3FFF] = rec;
                    USART1_RX_STA++;
                    if (USART1_RX_STA > USART_REC_LEN - 1)
                        USART1_RX_STA = 0;
                }
            }
        }
    }
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);

    // 1\0
    // 12\0
    // 123456789\0
    /* // SET 是中断状态
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
        r = USART_ReceiveData(USART1);
        USART_SendData(USART2, r);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET)
            ;
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    } */
}

void USART2_IRQHandler(void) {
    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
        u16 rec = USART_ReceiveData(USART2); // 从USART2模块的数据寄存器中读取接收到的数据

        // 判断接收是否完成, 即是否已经标记了 收到 '\n'
        if ((USART2_RX_STA & 0x8000) == 0) {
            // 判断有没有收到过 '\r'
            if (USART2_RX_STA & 0x4000) {
                // 数据接收不正常
                if (rec != 0x0A)
                    USART2_RX_STA = 0;
                else
                    USART2_RX_STA |= 0x8000; // 标记数据接收完成
            }
            else {
                // 表示接收到的是 \r
                if (rec == 0x0D)
                    USART2_RX_STA |= 0x4000; // 标记数据接收到\r
                else {
                    USART2_RX_BUF[USART2_RX_STA & 0x3FFF] = rec;
                    USART2_RX_STA++;
                    if (USART2_RX_STA > USART_REC_LEN - 1)
                        USART2_RX_STA = 0;
                }
            }
        }
    }
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);

    // SET 是中断状态
    /* if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
        r = USART_ReceiveData(USART2);
        USART_SendData(USART1, r);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET)
            ;
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    } */
}

/* void USART3_IRQHandler(void) {
    u16 r = 0;

    // SET 是中断状态
    if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET) {
        r = USART_ReceiveData(USART3);
        USART_SendData(USART3, r);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) != SET)
            ;
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
} */

int fputc(int ch, FILE* p) {
    if (USART_use == 1) {
        USART_SendData(USART2, (u16)ch);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET)
            ;
    }

    if (USART_use == 2) {
        USART_SendData(USART1, (u16)ch);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET)
            ;
    }

    return ch;
}

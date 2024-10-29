#include "stm32f10x_it.h"
#include "utils.h"
#include "usart.h"

#define USART_REC_LEN 200 // 最大串口缓冲区

s8 USART3_RX_BUF[USART_REC_LEN]; // 串口2的接收缓冲
u16 USART3_RX_STA;               // 串口2接收状态标记+接收个数

int main() {
    SysTick_init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 2级 优先级

    USART3_init(115200);
    ESP8266_init();
    delayMs(1000);
    ESP8266_serverConfig();

    while (1) {
    }
}

// 接收数据的中断
void USART3_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
        u16 rec = USART_ReceiveData(USART1); // 从USART1模块的数据寄存器中读取接收到的数据

        // 判断接收是否完成, 即是否已经标记了 收到 '\n'
        if ((USART3_RX_STA & 0x8000) == 0) {
            // 判断有没有收到过 '\r'
            if (USART3_RX_STA & 0x4000) {
                // 数据接收不正常
                if (rec != 0x0A)
                    USART3_RX_STA = 0;
                else                         // 数据正常, 即收到 \n
                    USART3_RX_STA |= 0x8000; // 标记数据接收完成
            }
            else {
                // 表示接收到的是 \r
                if (rec == 0x0D)
                    USART3_RX_STA |= 0x4000; // 标记数据接收到\r
                else {
                    // 存储 有效数据
                    USART3_RX_BUF[USART3_RX_STA & 0x3FFF] = rec;
                    USART3_RX_STA++;
                    if (USART3_RX_STA > USART_REC_LEN - 1)
                        USART3_RX_STA = 0;
                }
            }
        }
    }
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

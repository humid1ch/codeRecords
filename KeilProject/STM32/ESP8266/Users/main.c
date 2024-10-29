#include "stm32f10x_it.h"
#include "string.h"
#include "utils.h"
#include "usart.h"
#include "esp8266.h"
#include "led.h"

#define USART_REC_LEN 128 // 最大串口缓冲区

char USART3_RX_BUF[USART_REC_LEN] = {0}; // 串口3的接收缓冲
u16 USART3_RX_STA = 0;                   // 串口3接收状态标记+接收个数

char USART1_RX_BUF[USART_REC_LEN] = {0}; // 串口1的接收缓冲
u16 USART1_RX_STA = 0;                   // 串口1接收状态标记+接收个数

void ESP8266_serverConfig(void);

int main() {
    SysTick_init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 2级 优先级

    LED_init();
    USART1_init(115200);
    USART3_init(115200);
    ESP8266_init();
    delayMs(1000);
    ESP8266_serverConfig();

    while (1) {
        u16 dataLen = 0;
        if (USART1_RX_STA & 0x8000) {
            USART1_USE();
            printf("USART1 send: %s\r\n", USART1_RX_BUF);

            // 数据清零
            dataLen = USART1_RX_STA & 0x3FFF;
            for (u16 i = 0; i < dataLen; i++)
                USART1_RX_BUF[i] = 0;
            USART1_RX_STA = 0;
        }

        if (USART3_RX_STA & 0x8000) {
            if (strstr(USART3_RX_BUF, "LED_ON") != NULL)
                // 亮灯
                LED_writeByte(LEDS_PORT, 0x00, WRITE_L8);
            else if (strstr(USART3_RX_BUF, "LED_OFF") != NULL)
                // 灭灯
                LED_writeByte(LEDS_PORT, 0xFF, WRITE_L8);

            // 数据清零
            dataLen = USART3_RX_STA & 0x3FFF;
            for (u16 i = 0; i < dataLen; i++)
                USART3_RX_BUF[i] = 0;

            USART3_RX_STA = 0;

            /* USART1_USE();
            printf("USART3 rcv: %s\r\n", USART3_RX_BUF);

            // 数据清零
            dataLen = USART3_RX_STA & 0x3FFF;
            for (u16 i = 0; i < dataLen; i++) {
                USART_SendData(USART1, USART3_RX_BUF[i]);
                while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET)
                    ;

                USART3_RX_BUF[i] = 0;
            }

            USART_SendData(USART1, '\r');
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET)
                ;
            USART_SendData(USART1, '\n');
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET)
                ;
            USART3_RX_STA = 0; */
        }
    }
}

void ESP8266_serverConfig(void) {
    char cmdStr[128] = {0}; // 用于存储格式化命令

    sprintf(cmdStr, "AT+CWMODE=2\r\n");
    _Send_AT_Cmd(cmdStr);
    delayMs(1000);

    sprintf(cmdStr, "AT+CWSAP_DEF=\"%s\",\"%s\",%d,%d\r\n", WIFI_SSID, WIFI_PASSWD, WIFI_CHANNEL, WIFI_PSSWD_MODE);
    _Send_AT_Cmd(cmdStr);
    delayMs(1000);

    sprintf(cmdStr, "AT+CIPAP_DEF=\"%s\"\r\n", ESP_LOCAL_IP);
    _Send_AT_Cmd(cmdStr);
    delayMs(1000);

    sprintf(cmdStr, "AT+CIPMUX=1\r\n");
    _Send_AT_Cmd(cmdStr);
    delayMs(1000);

    sprintf(cmdStr, "AT+CIPSERVER=1,%d\r\n", ESP_SERVER_PORT);
    _Send_AT_Cmd(cmdStr);
    delayMs(1000);
}

void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
        u16 rec = USART_ReceiveData(USART1); // 从USART1模块的数据寄存器中读取接收到的数据

        // 判断接收是否完成, 即是否已经标记了 收到 '\n'
        if ((USART1_RX_STA & 0x8000) == 0) {
            // 判断有没有收到过 '\r'
            if (USART1_RX_STA & 0x4000) {
                // 数据接收不正常
                if (rec != '\n')
                    USART1_RX_STA = 0;
                else                         // 数据正常, 即收到 \n
                    USART1_RX_STA |= 0x8000; // 标记数据接收完成
            }
            else {
                // 表示接收到的是 \r
                if (rec == '\r')
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
}

// 接收数据的中断
void USART3_IRQHandler(void) {
    if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET) {
        u16 rec = USART_ReceiveData(USART3); // 从USART1模块的数据寄存器中读取接收到的数据

        // 判断接收是否完成, 即是否已经标记了 收到 '\n'
        if ((USART3_RX_STA & 0x8000) == 0) {
            // 判断有没有收到过 '\r'
            if (USART3_RX_STA & 0x4000) {
                // 数据接收不正常
                if (rec != '\n')
                    USART3_RX_STA = 0;
                else                         // 数据正常, 即收到 \n
                    USART3_RX_STA |= 0x8000; // 标记数据接收完成
            }
            else {
                // 表示接收到的是 \r
                if (rec == '\r')
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
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}

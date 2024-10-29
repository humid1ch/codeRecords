#include "esp8266.h"
#include "usart.h"

void ESP8266_init(void) {
    // 复位引脚 时钟使能
    RCC_APB2PeriphClockCmd(ESP_RST_RCC, ENABLE);
    // 使能引脚 时钟使能
    RCC_APB2PeriphClockCmd(ESP_CH_PD_RCC, ENABLE);

    GPIO_InitTypeDef RST_initStruct;
    RST_initStruct.GPIO_Pin = ESP_RST_PIN;
    RST_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    RST_initStruct.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_Init(ESP_RST_PORT, &RST_initStruct);

    GPIO_InitTypeDef CH_PD_initStruct;
    CH_PD_initStruct.GPIO_Pin = ESP_CH_PD_PIN;
    CH_PD_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    CH_PD_initStruct.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_Init(ESP_CH_PD_PORT, &CH_PD_initStruct);

    // 两引脚都 拉高使能
    ESP_RST = 1;
    ESP_CH_PD = 1;
}
void ESP8266_sendATCmd(const char* cmd) {
    // ESP8266 用串口3 的RX 和 TX引脚
    USART3_USE();
    printf("%s", cmd);

    // 向串口1发送, 调试用
    USART1_USE();
    printf("%s", cmd);
}

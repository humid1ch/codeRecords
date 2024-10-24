#include "led.h"

void LED_init(void) {
    // 时钟
    RCC_APB2PeriphClockCmd(LEDS_PORT_RCC, ENABLE);

    // GPIO
    GPIO_InitTypeDef GPIO_initStruct;

    GPIO_initStruct.GPIO_Pin = LED0_PIN | LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN | LED5_PIN | LED6_PIN | LED7_PIN;
    GPIO_initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LEDS_PORT, &GPIO_initStruct);

    u16 u16value = GPIO_ReadOutputData(LEDS_PORT);
    u16value &= 0x00FF;              // 低8位置1
    GPIO_Write(LEDS_PORT, u16value); // 写入到GPIOA组
}

// 参数: GPIOx 引脚组, value 要写入引脚的8位值, HLBits 写入高八位还是第八位
void LED_writeByte(GPIO_TypeDef* GPIOx, u8 value, u8 HL8Bits) {
    // 读出当前的引脚 16位 输出状态
    u16 u16value = GPIO_ReadOutputData(GPIOx);
    if (HL8Bits == WRITE_H8) {
        u16value &= 0x00FF;     // 高八位置零
        u16value |= value << 8; // 高八位赋值
    }

    if (HL8Bits == WRITE_L8) {
        u16value &= 0xFF00; // 第八位置零
        u16value |= value;  // 第八位赋值
    }

    GPIO_Write(GPIOx, u16value);
}

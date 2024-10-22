#include "stm32f10x_it.h"
#include "led.h"
#include "utils.h"

void ledPattern(void);

int main() {
    LED_init();

    while (1) {
        ledPattern();
    }
}

// 实现LED流水灯的功能
void ledPattern(void) {
    u8 i = 0;
    u8 mask = 0x01; //从最低位开始的掩码

    // 从最低位到最高位
    for (i = 0; i < 8; i++) {
        LED_writeByte(LED0_PORT, ~mask, WRITE_L8); // 使用按位取反操作来点亮当前LED
        delayMs(100);
        mask <<= 1;
    }

    // 从最高位到最低位
    mask = 0x80;
    for (i = 0; i < 8; i++) {
        LED_writeByte(LED0_PORT, ~mask, WRITE_L8); // 使用按位取反操作来点亮当前LED
        delayMs(100);
        mask >>= 1;
    }
}


#include "stm32f10x_it.h"
#include "utils.h"
#include "led_dots.h"

int main() {
    SysTick_init(72);
    LEDOTS_init();

    u8 u8HelloSmall[] = {0x01, 0x7D, 0x11, 0x11, 0x7D, 0x01, 0x01, 0x7D, 0x55, 0x55, 0x55, 0x01, 0x01, 0x7D, 0x05, 0x05, 0x05, 0x01, 0x01,
                         0x7D, 0x05, 0x05, 0x05, 0x01, 0x01, 0x7D, 0x45, 0x45, 0x7D, 0x01, 0x01, 0x7D, 0x11, 0x11, 0x7D, 0x01, 0x01};

    while (1) {
        LEDOTS_leftScroll(u8HelloSmall, sizeof(u8HelloSmall) / sizeof(u8HelloSmall[0]));
        // LEDOTS_colCirculate();
        LEDOTS_rowCirculate();
    }
}
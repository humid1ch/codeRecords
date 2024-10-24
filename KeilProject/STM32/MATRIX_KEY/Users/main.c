#include "stm32f10x_it.h"
#include "utils.h"
#include "matrix_key.h"
#include "led.h"

int main() {
    SysTick_init(72);
    LED_init();
    MKEY_init();

    u8 ledMask = 0x00;
    while (1) {
        u16 keyRes = MKEY_keyScan();
        if (keyRes != MKEY_NON_PRESSED) {
            ledMask = ~ledMask;
            LED_writeByte(LED0_PORT, ledMask, WRITE_L8);
        }
    }
}

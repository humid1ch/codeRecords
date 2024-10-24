#include "stm32f10x_it.h"
#include "utils.h"
#include "skey.h"
#include "led.h"

int main() {
    SysTick_init(72);
    LED_init();
    SKEY_init();

    u8 u8mask = 0xff;
    while (1) {
        u8 keyRes = SKEY_keyScan();
        if (keyRes != NON_KEY_PRESSED) {
            u8mask = ~u8mask;
            LED_writeByte(LED0_PORT, u8mask, WRITE_L8);
        }
    }
}

#include "stm32f10x_it.h"
#include "utils.h"
#include "smg.h"

u8 smgCode[8] = {0, 1, 2, 3, 4, 5, 6, 7};

int main() {
    SysTick_init(72);
    SMG_init();

    while (1) {
        SMG_display(smgCode);
    }
}

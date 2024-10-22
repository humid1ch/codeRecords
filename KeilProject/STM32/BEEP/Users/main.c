#include "stm32f10x_it.h"
#include "utils.h"
#include "beep.h"

int main() {
    SysTick_init(72);
    BEEP_init();

    BEEP_on(2);

    while (1) {
    }
}

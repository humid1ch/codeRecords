#include "stm32f10x_it.h"
#include "utils.h"
#include "lcd9648.h"

int main() {
	SysTick_init(72);
	LCD9648_init();

    while (1) {
		LCD9648_clear();
		delayMs(1000);

		LCD9648_showChinese(0, 1, 16, "天王老子");
		delayMs(1000);
    }
}

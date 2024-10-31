#include "stm32f10x_it.h"
#include "utils.h"
#include "pic.h"
#include "ssd1306.h"
#include "oled.h"

int main() {
    SysTick_init(72);
    // 先对驱动进行初始化
    SSD1306_init();
    OLED_init();

    while (1) {
        OLED_clear();
        OLED_showImage(gImage_pic);

        OLED_clear();                               
        OLED_showChinese(0, 0, 24, "天王老子");
        OLED_showChinese(0, 5, 16, "天王老子");

        OLED_clear();
        OLED_showString(0, 0, "tianwanglaozi.");
        OLED_showString(0, 3, "TIANWANGLAOZI.");
    }
}

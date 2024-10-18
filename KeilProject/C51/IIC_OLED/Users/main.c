#include "reg52.h"
#include "pic.h"
#include "oled.h"

void main() {
    OLED_init();
    delayNMs(200);

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

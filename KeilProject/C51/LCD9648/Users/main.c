#include "reg52.h"
#include "util.h"
#include "lcd9648.h"
#include "pic.h"

void main() {
    LCD9648_init();
    delayNMs(200);

    //清屏
    LCD9648_clear();
    delayNMs(200);

    // 显示图片
    // LCD9648_showImage(gImage_pic);
    // 显示24x24字符串
    LCD9648_showChinese(0, 0, 24, "天王老子");
    // 显示16x16字符串
    // LCD9648_showChinese(0, 0, 16, "天王老子");

    delayNMs(2000);
    while (1)
        ;
}

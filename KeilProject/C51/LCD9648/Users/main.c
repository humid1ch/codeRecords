#include "reg52.h"
#include "util.h"
#include "lcd9648.h"
#include "pic.h"

void main() {
    LCD9648_Init();

    delayNMs(2000);
    //清屏
    LCD9648_Clear();
    delayNMs(2000);

    //显示图片
    LCD_SHOW_Image(gImage_pic);

    delayNMs(2000);
    while (1)
        ;
}
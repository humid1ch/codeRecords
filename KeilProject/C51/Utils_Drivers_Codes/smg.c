#include "smg.h"
#include "reg52.h"

// 3->8译码器的三个引脚
// 板子上 右边数码管的是较小值
sbit SMG_22 = P2 ^ 2;
sbit SMG_23 = P2 ^ 3;
sbit SMG_24 = P2 ^ 4;

void SMG_Init() {}

// 8个 1位数码管选择函数
void SMG1() {
    SMG_24 = 1;
    SMG_23 = 1;
    SMG_22 = 1;
}
void SMG2() {
    SMG_24 = 1;
    SMG_23 = 1;
    SMG_22 = 0;
}
void SMG3() {
    SMG_24 = 1;
    SMG_23 = 0;
    SMG_22 = 1;
}
void SMG4() {
    SMG_24 = 1;
    SMG_23 = 0;
    SMG_22 = 0;
}
void SMG5() {
    SMG_24 = 0;
    SMG_23 = 1;
    SMG_22 = 1;
}
void SMG6() {
    SMG_24 = 0;
    SMG_23 = 1;
    SMG_22 = 0;
}
void SMG7() {
    SMG_24 = 0;
    SMG_23 = 0;
    SMG_22 = 1;
}
void SMG8() {
    SMG_24 = 0;
    SMG_23 = 0;
    SMG_22 = 0;
}

// 数码管选择函数 数组
smgSelFunc smgFuncs[] = {SMG1, SMG2, SMG3, SMG4, SMG5, SMG6, SMG7, SMG8};

// 板子使用 共阴数码管, 各引脚给高电平点亮边
// a b c d e f g dp  低->高
// dp g f e d c b a
// 0 1 2 3 4 5 6 7 8 9 A B C D E F . 空
unsigned char smgNum[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,  0x80, 0x00};

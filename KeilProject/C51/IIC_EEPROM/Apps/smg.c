#include "smg.h"
#include "reg52.h"

// 3->8译码器的三个引脚
// 板子上 右边数码管的是较小值
sbit SMG_22 = P2 ^ 2;
sbit SMG_23 = P2 ^ 3;
sbit SMG_24 = P2 ^ 4;

// 8个 1位数码管选择函数
void smg1() {
    SMG_24 = 1;
    SMG_23 = 1;
    SMG_22 = 1;
}
void smg2() {
    SMG_24 = 1;
    SMG_23 = 1;
    SMG_22 = 0;
}
void smg3() {
    SMG_24 = 1;
    SMG_23 = 0;
    SMG_22 = 1;
}
void smg4() {
    SMG_24 = 1;
    SMG_23 = 0;
    SMG_22 = 0;
}
void smg5() {
    SMG_24 = 0;
    SMG_23 = 1;
    SMG_22 = 1;
}
void smg6() {
    SMG_24 = 0;
    SMG_23 = 1;
    SMG_22 = 0;
}
void smg7() {
    SMG_24 = 0;
    SMG_23 = 0;
    SMG_22 = 1;
}
void smg8() {
    SMG_24 = 0;
    SMG_23 = 0;
    SMG_22 = 0;
}

// 数码管选择函数 数组
smgSelFunc smgFuncs[] = {smg1, smg2, smg3, smg4, smg5, smg6, smg7, smg8};

// 板子使用 共阴数码管, 各引脚给高电平点亮边
// a b c d e f g dp  低->高
// dp g f e d c b a
unsigned char smgNum[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00};

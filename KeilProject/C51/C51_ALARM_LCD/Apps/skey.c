#include "reg52.h"
#include "skey.h"

// 四个按键, 连接引脚默认为高电平, 按下按键 与地导通, 变为低电平
sbit SKEY1 = P3 ^ 1;
sbit SKEY2 = P3 ^ 0;
sbit SKEY3 = P3 ^ 2;
sbit SKEY4 = P3 ^ 3;

void SKEY_Init() {}

u8 SKEY_Scan() {
    // 用 低四位表示 每个按键是否被按下
    // 对应位 被按下, 就将对应位设置为1
    // 低四位中 高位->低位 分别对应SKEY1 -> S_KEY4
    u8 keyState = NON_KEY_PRESSED;

    if (SKEY1 == 0 || SKEY2 == 0 || SKEY3 == 0 || SKEY4 == 0) {
        delayNMs(2); // 消抖, 确认是否被按下
        if (SKEY1 == 0)
            keyState = SKEY1_PRESSED;

        if (SKEY2 == 0)
            keyState = SKEY2_PRESSED;

        if (SKEY3 == 0)
            keyState = SKEY3_PRESSED;

        if (SKEY4 == 0)
            keyState = SKEY4_PRESSED;
    }

    // 只要有按键是0, 就表示还有按键没有被抬起, 就不返回
    while (SKEY1 == 0 || SKEY2 == 0 || SKEY3 == 0 || SKEY4 == 0)
        ;

    // 返回keyState
    return keyState;
}
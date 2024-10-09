#include "reg52.h"
#include "skey.h"

// 四个按键, 连接引脚默认为高电平, 按下按键 与地导通, 变为低电平
sbit S_KEY1 = P3 ^ 1;
sbit S_KEY2 = P3 ^ 0;
sbit S_KEY3 = P3 ^ 2;
sbit S_KEY4 = P3 ^ 3;

u8 keyScan() {
    // 用 低四位表示 每个按键是否被按下
    // 对应位 被按下, 就将对应位设置为1
    // 低四位中 高位->低位 分别对应S_KEY1 -> S_KEY4
    u8 keyState = NON_KEY_PRESSED;

    if (S_KEY1 == 0 || S_KEY2 == 0 || S_KEY3 == 0 || S_KEY4 == 0) {
        delayNMs(2); // 消抖, 确认是否被按下
        if (S_KEY1 == 0)
            keyState = S_KEY1_PRESSED;

        if (S_KEY2 == 0)
            keyState = S_KEY2_PRESSED;

        if (S_KEY3 == 0)
            keyState = S_KEY3_PRESSED;

        if (S_KEY4 == 0)
            keyState = S_KEY4_PRESSED;
    }

    // 只要有按键是0, 就表示还有按键没有被抬起, 就不返回
    while (S_KEY1 == 0 || S_KEY2 == 0 || S_KEY3 == 0 || S_KEY4 == 0)
        ;

    // 返回keyState
    return keyState;
}
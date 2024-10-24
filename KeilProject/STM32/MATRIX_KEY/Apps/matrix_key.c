#include "matrix_key.h"
#include "utils.h"

void MKEY_init() {
    // 行引脚 时钟使能
    RCC_APB2PeriphClockCmd(KEY_HS_PORT_RCC, ENABLE);
    // 列引脚 时钟使能
    RCC_APB2PeriphClockCmd(KEY_LS_PORT_RCC, ENABLE);

    // 行 或 列需要有一方设置为 推挽输出模式, 使其可以通过代码控制高低电平
    GPIO_InitTypeDef KEY_HS_initStruct;
    KEY_HS_initStruct.GPIO_Pin = KEY_H1_PIN | KEY_H2_PIN | KEY_H3_PIN | KEY_H4_PIN;
    KEY_HS_initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    KEY_HS_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY_HS_PORT, &KEY_HS_initStruct);

    // 行 或 列另一方, 需要设置为下拉输入模式, 因为要通过按下按键 引起电平变化
    // 为什么是 下拉输入而不是上拉输入, 因为如果设置为上拉模式, 设置为推挽输出的引脚的电平变化, 可能会影响此时的输入电平
    GPIO_InitTypeDef KEY_LS_initStruct;
    KEY_LS_initStruct.GPIO_Pin = KEY_L1_PIN | KEY_L2_PIN | KEY_L3_PIN | KEY_L4_PIN;
    KEY_LS_initStruct.GPIO_Mode = GPIO_Mode_IPD;
    KEY_LS_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY_LS_PORT, &KEY_LS_initStruct);
}

static u16 _MKEY_whichLTrue(void) {
    delayMs(10);

    if (KEY_L1 && !KEY_L2 && !KEY_L3 && !KEY_L4)
        return 0x0001;
    if (!KEY_L1 && KEY_L2 && !KEY_L3 && !KEY_L4)
        return 0x0002;
    if (!KEY_L1 && !KEY_L2 && KEY_L3 && !KEY_L4)
        return 0x0004;
    if (!KEY_L1 && !KEY_L2 && !KEY_L3 && KEY_L4)
        return 0x0008;

    return 0x0000;
}

u16 MKEY_keyScan(void) {
    u16 keyRes = MKEY_NON_PRESSED;
    u8 i = 0;
    do {
        KEY_H1 = KEY_H2 = KEY_H3 = KEY_H4 = 0;

        if (i == 0) {
            KEY_H1 = 1;
            keyRes = _MKEY_whichLTrue() << (i * 4);
        }
        else if (i == 1) {
            KEY_H2 = 1;
            keyRes = _MKEY_whichLTrue() << (i * 4);
        }
        else if (i == 2) {
            KEY_H3 = 1;
            keyRes = _MKEY_whichLTrue() << (i * 4);
        }
        else if (i == 3) {
            KEY_H4 = 1;
            keyRes = _MKEY_whichLTrue() << (i * 4);
        }

        while (KEY_L1 || KEY_L2 || KEY_L3 || KEY_L4)
            ; //等待按键弹起
    } while (++i < 4 && !keyRes);

    return keyRes;
}

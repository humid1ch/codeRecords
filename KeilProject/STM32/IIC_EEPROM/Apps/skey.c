#include "skey.h"
#include "utils.h"

void SKEY_init(void) {
    RCC_APB2PeriphClockCmd(S_KEY_RCC, ENABLE);

    // 数码管位选引脚IO功能开启
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    // 关闭引脚的 JTAG功能
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);

    GPIO_InitTypeDef skeyInitStruct;
    skeyInitStruct.GPIO_Pin = S_KEY1_PIN | S_KEY2_PIN | S_KEY3_PIN | S_KEY4_PIN;
    skeyInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    skeyInitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(S_KEY_PORT, &skeyInitStruct);

}

u8 SKEY_keyScan(void) {
    // 用 低四位表示 每个按键是否被按下
    // 对应位 被按下, 就将对应位设置为1
    // 低四位中 高位->低位 分别对应S_KEY1 -> S_KEY4
    u8 keyState = NON_KEY_PRESSED;

    if (S_KEY1 == 0 || S_KEY2 == 0 || S_KEY3 == 0 || S_KEY4 == 0) {
        delayMs(10); // 消抖, 确认是否被按下
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

#include "beep.h"
#include "utils.h"

void BEEP_init(void) {
    // 时钟
    RCC_APB2PeriphClockCmd(BEEP_PORT_RCC, ENABLE);

    // GPIO
    GPIO_InitTypeDef GPIO_initStruct;

    GPIO_initStruct.GPIO_Pin = BEEP_PIN;
    GPIO_initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BEEP_PORT, &GPIO_initStruct);
}

/* // 蜂鸣器响多少秒
void BEEP_on(u8 seconds) {
    u16 cnt = FEQUENCY_S * seconds; // 脉冲一定秒数 需要脉冲的次数

    while (cnt--)
        _BEEP_pulse500Us();
}
 */
// 设置蜂鸣器 500us为一次脉冲, 占空比50%, 即 高250us, 低250us
// 约 2000次脉冲/s
void BEEP_pulse50Us(void) {
    BEEP = 0;
    delayUs(25);
    BEEP = 1;
    delayUs(25);
}

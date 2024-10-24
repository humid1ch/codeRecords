#include "step_motor.h"
#include "utils.h"

void STEP_MOTOR_init(void) {
    // 引脚组 时钟使能
    RCC_APB2PeriphClockCmd(MOTO_INS_PORT_RCC, ENABLE);

    GPIO_InitTypeDef MOTO_initStruct;
    MOTO_initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    MOTO_initStruct.GPIO_Pin = MOTO_A_PIN | MOTO_B_PIN | MOTO_C_PIN | MOTO_D_PIN;
    MOTO_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTO_INS_PORT, &MOTO_initStruct);
}

// states: 启动 和 停止 , clock: 顺时针 或 逆时针, speed: 速度(1, 2), angle: 角度(0 ~ 360)
void STEP_MOTOR_run(u8 states, u8 clock, u8 speed, u16 angle) {
    if ((states != MOTOR_RUN && states != MOTOR_STOP) || (clock != CLOCK_WISE && clock != COUNTER_CLOCK_WISE) || (speed != HIGH_SPEED && speed != LOW_SPEED))
        return;

    // 停止状态 或 转 0° 直接退出
    if (states == MOTOR_STOP || angle == 0)
        return;

    if (angle > 360)
        angle = 360;

    // 如果是顺时针, 就8递减循环执行 步进
    // 顺时针 控制i--, i > 0, 逆时针 控制 i++, i < 9
    for (u8 i = (clock == CLOCK_WISE ? 8 : 1); (clock == CLOCK_WISE ? i > 0 : i < 9); (clock == CLOCK_WISE ? i-- : i++)) {
        switch (i) {
        case 1:
            MOTO_A = MOTO_B = MOTO_C = 1;
            MOTO_D = 0;
            break;
        case 2:
            MOTO_A = MOTO_B = 1;
            MOTO_D = MOTO_C = 0;
            break;
        case 3:
            MOTO_A = MOTO_B = MOTO_D = 1;
            MOTO_C = 0;
            break;
        case 4:
            MOTO_A = MOTO_D = 1;
            MOTO_B = MOTO_C = 0;
            break;
        case 5:
            MOTO_A = MOTO_C = MOTO_D = 1;
            MOTO_B = 0;
            break;
        case 6:
            MOTO_C = MOTO_D = 1;
            MOTO_A = MOTO_B = 0;
            break;
        case 7:
            MOTO_B = MOTO_C = MOTO_D = 1;
            MOTO_A = 0;
            break;
        case 8:
            MOTO_B = MOTO_C = 1;
            MOTO_A = MOTO_D = 0;
            break;
        }
        delayUs(2000 / speed);
    }
}

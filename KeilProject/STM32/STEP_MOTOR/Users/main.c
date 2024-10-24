#include "stm32f10x_it.h"
#include "utils.h"
#include "step_motor.h"
#include "skey.h"

// 状态 默认停止
u8 motorState = MOTOR_STOP;
// 默认顺时针
u8 isClockWise = CLOCK_WISE;
// 默认低速
u8 motorSpeed = LOW_SPEED;

int main() {
    SysTick_init(72);
    SKEY_init();
    STEP_MOTOR_init();

    while (1) {
        u8 keyRes = SKEY_keyScan();
        switch (keyRes) {
        case S_KEY1_PRESSED:
            if (motorState == MOTOR_RUN)
                motorState = MOTOR_STOP;
            else
                motorState = MOTOR_RUN;
            break;

        case S_KEY2_PRESSED:
            if (isClockWise == CLOCK_WISE)
                isClockWise = COUNTER_CLOCK_WISE;
            else
                isClockWise = CLOCK_WISE;
            break;

        case S_KEY3_PRESSED:
            motorSpeed = HIGH_SPEED;
            break;

        case S_KEY4_PRESSED:
            motorSpeed = LOW_SPEED;
            break;

        default:
            break;
        }

        STEP_MOTOR_run(motorState, isClockWise, motorSpeed, 1);
    }
}

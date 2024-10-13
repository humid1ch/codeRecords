#include "reg52.h"
#include "util.h"
#include "pwm.h"

void main() {
    u16 u16period = 1000; //单位us
    u8 u8duty = 0;        //脉冲宽度
    u8 u8dir = 0;         //0表示duty由低到高，1：表示duty由高到低

    PWM_Init();

    while (1) {
        if (u8dir == 0) {
            u8duty++; //脉冲占空比递增
            if (u8duty == 70)
                u8dir = 1; //
        }
        else {
            u8duty--; //脉冲占空比递减
            if (u8duty == 0)
                u8dir = 0;
        }
        PWM_SetPULES(u16period, u8duty);
        delayNMs(10);
    }
}
#include "reg52.h"
#include "pwm.h"

sbit PWM_PULES = P2 ^ 1;

void PWM_Init() {}

// u16period, 表示 一个脉冲时长 [100, 1000] us
// u8duty, 表示 高电平占空比 [0, 100]
void PWM_SetPULES(u16 u16period, u8 u8duty) {
    u16 highWidth = 0;
    u16 lowWidth = 0;

    u16period = u16period < 100 ? 100 : u16period;
    u16period = u16period > 1000 ? 1000 : u16period;
    u8duty = u8duty > 100 ? 100 : u8duty;
    
    highWidth = u16period * u8duty / 100;
    lowWidth = u16period - highWidth;
    
    PWM_PULES = 1;
    delayN10Us(highWidth / 10);
    PWM_PULES = 0;
    delayN10Us(lowWidth / 10);
}
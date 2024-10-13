#ifndef __PWM_H__
#define __PWM_H__

#include "util.h"

void PWM_Init();
void PWM_SetPULES(u16 u16period, u8 u8duty);

#endif
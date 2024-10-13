#ifndef __INFRARED_H__
#define __INFRARED_H__

#include "util.h"

extern u8 gIred_code[4]; //NEC码（地址码，地址反码，控制码，控制反码）

void IRD_Init();

#endif
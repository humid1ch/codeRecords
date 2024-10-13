#ifndef __XPT2046_H__
#define __XPT2046_H__

#include "util.h"

void XPT2046_Init();
u16 XPT2046_GetValue(u8 u8cmd);

#endif
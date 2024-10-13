#ifndef __SKEY_H__
#define __SKEY_H__

#include "util.h"

#define NON_KEY_PRESSED 0x00
#define S_KEY1_PRESSED 0x08
#define S_KEY2_PRESSED 0x04
#define S_KEY3_PRESSED 0x02
#define S_KEY4_PRESSED 0x01

// 按键扫描
u8 keyScan();

#endif
#ifndef __SKEY_H__
#define __SKEY_H__

#include "util.h"

#define NON_KEY_PRESSED 0x00
#define SKEY1_PRESSED 0x08
#define SKEY2_PRESSED 0x04
#define SKEY3_PRESSED 0x02
#define SKEY4_PRESSED 0x01

void SKEY_Init();
// 按键扫描
u8 SKEY_Scan();

#endif
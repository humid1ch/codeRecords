#ifndef __AT24C02_H__
#define __AT24C02_H__

#include "util.h"

// AT24C02读写驱动接口 (EEPROM)
#define ADDR_W 0xA0
#define ADDR_R 0xA1

void at24c02Init();
u8 at24c02WriteByte(u8 u8addr, u8 u8data);
u8 at24c02ReadByte(u8 u8addr);

#endif

#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "util.h"

void DS18B20_Init();
void DS18B20_ReadTemperature(u16* u16pdata);

#endif
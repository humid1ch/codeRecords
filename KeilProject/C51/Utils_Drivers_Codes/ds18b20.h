#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "util.h"

void ds18b20Init();
void ds18b20ReadTemperature(u16* u16pdata);

#endif
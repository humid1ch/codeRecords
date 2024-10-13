#include "ds18b20.h"
#include "one_write.h"

void DS18B20_Init() {
    ONE_WRITE_Init();
}

void DS18B20_ReadTemperature(u16* u16pdata) {
    u16 u16data = 0;
    ONE_WRITE_Reset();

    ONE_WRITE_WriteByte(0xCC);

    ONE_WRITE_WriteByte(0x44);

    delayMs();

    ONE_WRITE_Reset();

    ONE_WRITE_WriteByte(0xCC);

    ONE_WRITE_WriteByte(0xBE);

    *u16pdata = ONE_WRITE_ReadByte();
    u16data = ONE_WRITE_ReadByte();
    *u16pdata |= (u16data << 8);
}
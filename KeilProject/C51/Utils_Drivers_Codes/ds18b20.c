#include "ds18b20.h"
#include "one_write.h"

void ds18b20Init() {}

void ds18b20ReadTemperature(u16* u16pdata) {
    u16 u16data = 0;
    oneWriteReset();

    oneWriteWriteByte(0xCC);

    oneWriteWriteByte(0x44);

    delayMs();

    oneWriteReset();

    oneWriteWriteByte(0xCC);

    oneWriteWriteByte(0xBE);

    *u16pdata = oneWriteReadByte();
    u16data = oneWriteReadByte();
    *u16pdata |= (u16data << 8);
}
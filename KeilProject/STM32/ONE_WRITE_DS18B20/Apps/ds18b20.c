#include "ds18b20.h"
#include "one_write.h"

void DS18B20_init() {
    OW_init();
}

void DS18B20_readTemperature(u16* u16pdata) {
    u16 u16data = 0;
    OW_reset();

    OW_writeByte(0xCC);

    OW_writeByte(0x44);

    delayMs(1);

    OW_reset();

    OW_writeByte(0xCC);

    OW_writeByte(0xBE);

    // 先读低位
    *u16pdata = OW_readByte();
    // 再读高位
    u16data = OW_readByte();
    *u16pdata |= (u16data << 8);
}

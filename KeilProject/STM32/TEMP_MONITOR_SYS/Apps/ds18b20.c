#include "ds18b20.h"
#include "one_write.h"

void DS18B20_init(void) {
    OW_init();
}

u16 DS18B20_readTemperature(void) {
    u16 u16result = 0;
    u16 u16data = 0;
    OW_reset();

    OW_writeByte(0xCC);

    OW_writeByte(0x44);

    delayMs(1);

    OW_reset();

    OW_writeByte(0xCC);

    OW_writeByte(0xBE);

    // 先读低位
    u16result = OW_readByte();
    // 再读高位
    u16data = OW_readByte();
    u16result |= (u16data << 8);

    return u16result;
}

#include "stm32f10x_it.h"
#include "utils.h"
#include "ds18b20.h"
#include "smg.h"

void temperatureDisplay(float temperature, u8 negFlag) {
    u16 tempInt = (u16)(temperature *= 10);
    u8 mod = 0;
    u8 smgCode[3] = {0};

    char cnt = 2;
    while (tempInt) {
        mod = tempInt % 10;
        tempInt /= 10;
        smgCode[cnt--] = mod;
    }

    cnt = 0;
    for (cnt = 0; cnt < 3; cnt++) {
        if (negFlag) {
            smgFuncs[0](); // 负数选择
            SMG_writeByte(SMG_SEG_PORT, 0x40, WRITE_L8);
            delayMs(2);
            SMG_writeByte(SMG_SEG_PORT, 0x00, WRITE_L8);
        }

        smgFuncs[cnt + 1]();
        SMG_writeByte(SMG_SEG_PORT, smgNum[smgCode[cnt]], WRITE_L8);
        delayMs(2);
        SMG_writeByte(SMG_SEG_PORT, 0x00, WRITE_L8);

        // C
        smgFuncs[4]();
        SMG_writeByte(SMG_SEG_PORT, smgNum[12], WRITE_L8);
        delayMs(2);
        SMG_writeByte(SMG_SEG_PORT, 0x00, WRITE_L8);

        if (cnt == 1) {
            smgFuncs[cnt + 1]();
            SMG_writeByte(SMG_SEG_PORT, smgNum[16], WRITE_L8);
            delayMs(2);
            SMG_writeByte(SMG_SEG_PORT, 0x00, WRITE_L8);
        }
    }
}

int main() {
    u16 u16data = 0; // 接收 ds18b20 获取的值
    u8 negFlag = 0;
    float temperature = 0.0;

    SysTick_init(72);

    DS18B20_init();
    SMG_init();

    while (1) {
        DS18B20_readTemperature(&u16data);

        // 温度负数修正
        if (u16data & 0xF800) {
            u16data = (~u16data) + 1;
            negFlag = 1;
        }

        temperature = u16data * 0.0625;

        temperatureDisplay(temperature, negFlag);

        negFlag = 0;
    }
}

#include "reg52.h"
#include "smg.h"
#include "ds18b20.h"
#include "util.h"

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
            SMG = 0x40;    // G
            delayNMs(2);
            SMG = 0x00;
        }

        smgFuncs[cnt + 1]();
        SMG = smgNum[smgCode[cnt]];
        delayNMs(2);
        SMG = 0x00;

        if (cnt == 1) {
            smgFuncs[cnt + 1]();
            SMG = smgNum[17];
            delayNMs(2);
            SMG = 0x00;
        }
    }
}

void main() {
    u16 u16data = 0; // 接收 ds18b20 获取的值
    u8 negFlag = 0;
    float temperature = 0.0;

    smgInit();
    ds18b20Init();

    while (1) {
        ds18b20ReadTemperature(&u16data);

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
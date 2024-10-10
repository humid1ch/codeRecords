#include "reg52.h"
#include "smg.h"
#include "util.h"
#include "ds1302.h"

void SMG_Display() {
    u8 i = 0, j = 0;

    for (i = 0, j = 0; i < 3; i++) {
        smgFuncs[j]();
        j++;
        SMG = smgNum[gDS1302_Time[2 - i] / 16]; // 十位
        delayNMs(2);
        SMG = 0x00;

        smgFuncs[j]();
        j++;
        SMG = smgNum[gDS1302_Time[2 - i] % 16]; // 个位
        delayNMs(2);
        SMG = 0x00;

        if (j < 7) {
            smgFuncs[j]();
            j++;
            SMG = 0x40; // -
            delayNMs(2);
            SMG = 0x00;
        }
    }
}

void main() {

    SMG_Init();
    DS1302_Init();

    while (1) {
        DS1302_GetTime();
        SMG_Display();
        delayMs();
    }
}
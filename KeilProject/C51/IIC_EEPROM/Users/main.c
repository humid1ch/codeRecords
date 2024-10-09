#include "reg52.h"
#include "smg.h"
#include "skey.h"
#include "iic.h"
#include "at24c02.h"
#include "util.h"
#include "uart.h"

// 显示计数数据
u8 displayData = 0;
// 显示技术数组
u8 displayNums[3] = {0};
// smg显示
void smgDisplay() {
    // 清空数组
    // 把数据每位放入数组
    u8 dataTemp = displayData;
    u8 dataUnit = 0;
    u8 i = 2;

    displayNums[0] = 0;
    displayNums[1] = 0;
    displayNums[2] = 0;
    while (dataTemp) {
        dataUnit = dataTemp % 10;
        dataTemp /= 10;

        displayNums[i--] = dataUnit;
    }

    for (i = 0; i <= 2; i++) {
        smgFuncs[i]();
        SMG = smgNum[displayNums[i]];
        delayNMs(4);
        SMG = smgNum[16];
    }
}

void main() {
    u8 sKeyRes = 0;
    u8 at24c02Addr = 0;

    at24c02Init();
    uartInit();

    while (1) {
        smgDisplay();
        sKeyRes = keyScan();

        switch (sKeyRes) {
        case S_KEY1_PRESSED:
            if (at24c02WriteByte(at24c02Addr, displayData))
                printf("AT24C02 write data failed!\r\n");
            else
                printf("AT24C02 write data success! data: %u\r\n", displayData);
            break;

        case S_KEY2_PRESSED:
            if ((displayData = at24c02ReadByte(at24c02Addr)) == 0) {
                printf("read data failed!\r\n");
            }
            else {
                printf("read data success! data: %u\r\n", displayData);
            }
            break;

        case S_KEY3_PRESSED:
            if (displayData < 255)
                displayData++;

            printf("data + 1: %u\r\n", displayData);
            break;

        case S_KEY4_PRESSED:
            displayData = 0;
            break;
        }
    }
}
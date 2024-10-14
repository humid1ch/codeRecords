#include "reg52.h"
#include "infrared.h"
#include "smg.h"

void main() {
    u8 u8code[8] = {0};

    IRD_Init();
    SMG_Init();

    while (1) {
        u8code[0] = smgNum[gIred_code[2] / 16];
        u8code[1] = smgNum[gIred_code[2] % 16];
        u8code[2] = smgNum[16];
        SMG_Display(u8code);
        delayMs();
    }
}
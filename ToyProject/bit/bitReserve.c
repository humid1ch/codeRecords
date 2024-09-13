#include <stdio.h>

void sendByte(unsigned char data) {
    unsigned char bit = 0;
    for (int i = 7; i >= 0; i--) {
        bit = (data & (0x01 << i)) ? 1 : 0;
        printf("%d ", bit);
    }
    printf("\n");
}

unsigned char reserveBits(unsigned char data) {
    unsigned char ret = 0;
    for (int i = 0; i < 8; i++) {
        ret |= ((data >> i) & 0x01) << (7 - i);
    }

    return ret;
}

int main() {
    while (1) {
        unsigned char num = 0;
        scanf("%u", &num);
        sendByte(num);

        unsigned char ret = reserveBits(num);
        printf("%u\n", ret);
        sendByte(ret);
    }

    return 0;
}

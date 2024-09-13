#include <stdio.h>

void sendByte(unsigned char data) {
    unsigned char bit = 0;
    for (int i = 7; i >= 0; i--) {
        bit = (data & (0x01 << i)) ? 1 : 0;
        printf("%d ", bit);
    }
    printf("\n");
}

int main() {
    while (1) {
        unsigned char num = 0;
        scanf("%u", &num);

        sendByte(num);
    }

    return 0;
}

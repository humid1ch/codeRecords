#include <stdio.h>

int main() {
    unsigned int value = 0x12345678;

    unsigned char v1 = (value >> 24) & 0xFF;
    unsigned char v2 = (value >> 16) & 0xFF;
    unsigned char v3 = (value >> 8) & 0xFF;
    unsigned char v4 = value & 0xFF;

    printf("%#x, %#x, %#x, %#x\n", v1, v2, v3, v4);

    unsigned int valueo = 0;
    
    valueo |= v1;
    valueo <<= 8;
    valueo |= v2;
    valueo <<= 8;
    valueo |= v3;
    valueo <<= 8;
    valueo |= v4;

    printf("%#x, %#x\n", value, valueo);
        
    return 0;
}

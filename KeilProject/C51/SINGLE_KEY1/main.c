#include "reg52.h"
#include "delay10Us.h"

// 四个按键, 连接引脚默认为高电平, 按下按键 与地导通, 变为低电平
sbit S_KEY1 = P3 ^ 1;
sbit S_KEY2 = P3 ^ 0;
sbit S_KEY3 = P3 ^ 2;
sbit S_KEY4 = P3 ^ 3;

sbit LED1 = P2 ^ 0;
sbit LED2 = P2 ^ 1;
sbit LED3 = P2 ^ 2;
sbit LED4 = P2 ^ 3;


unsigned char keyScan() {
    if (S_KEY1 == 0) {
        delay10Us();
        if (S_KEY1 == 0){
            return 1;
        }
    }
    
    if (S_KEY2 == 0) {
        delay10Us();
        if (S_KEY2 == 0){
            return 2;
        }
    }

    if (S_KEY3 == 0) {
        delay10Us();
        if (S_KEY3 == 0){
            return 3;
        }
    }

    if (S_KEY4 == 0) {
        delay10Us();
        if (S_KEY4 == 0){
            return 4;
        }
    }
    
	// 无按键被按下, 返回0
	return 0;
}

void main() {
	LED1 = LED2 = LED3 = LED4 = 1;
	while (1) {
		unsigned char ret = keyScan();
		switch (ret) {
		case 1:
			LED1 = ~LED1;
			break;
		case 2:
			LED2 = ~LED2;
			break;
		case 3:
			LED3 = ~LED3;
			break;
		case 4:
			LED4 = ~LED4;
			break;
		case 0:
			LED1 = 1;
			LED2 = 1;
			LED3 = 1;
			LED4 = 1;
			break;

		default:
			break;
		}
	}
}
#ifndef __SMG_H__
#define __SMG_H__

typedef void (*smgSelFunc)();

extern smgSelFunc smgFuncs[];

extern unsigned char smgNum[];

// 8位数码管引脚
#define SMG P0

// 8个 1位数码管选择函数
void smg1();
void smg2();
void smg3();
void smg4();
void smg5();
void smg6();
void smg7();
void smg8();

#endif
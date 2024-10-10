#ifndef __SMG_H__
#define __SMG_H__

typedef void (*smgSelFunc)();

extern smgSelFunc smgFuncs[];

extern unsigned char smgNum[];

// 8位数码管引脚
#define SMG P0

void SMG_Init();

// 8个 1位数码管选择函数
void SMG1();
void SMG2();
void SMG3();
void SMG4();
void SMG5();
void SMG6();
void SMG7();
void SMG8();

#endif
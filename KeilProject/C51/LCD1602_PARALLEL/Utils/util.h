#ifndef __UTIL_H__
#define __UTIL_H__

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

void delay10Us();
void delayN10Us(u16 n);

void delayMs();
void delayNMs(u16 n);

#endif
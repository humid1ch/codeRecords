#ifndef __MATRIX_KEY__
#define __MATRIX_KEY__

#include "stm32f10x.h"

// 位带操作, 实现 51 类似的 GPIO 控制功能
// IO 口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long*)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
#define GPIOB_IDR_Addr (GPIOB_BASE + 8)
#define GPIOB_ODR_Addr (GPIOB_BASE + 12)
// 输入
#define PBin(n) BIT_ADDR(GPIOB_IDR_Addr, n)
// 输出
#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr, n)

#define KEY_H1 PBout(8)
#define KEY_H2 PBout(9)
#define KEY_H3 PBout(10)
#define KEY_H4 PBout(11)

#define KEY_L1 PBin(12)
#define KEY_L2 PBin(13)
#define KEY_L3 PBin(14)
#define KEY_L4 PBin(15)

#define KEY_HS_PORT_RCC RCC_APB2Periph_GPIOB
#define KEY_HS_PORT GPIOB
#define KEY_H1_PIN GPIO_Pin_8
#define KEY_H2_PIN GPIO_Pin_9
#define KEY_H3_PIN GPIO_Pin_10
#define KEY_H4_PIN GPIO_Pin_11

#define KEY_LS_PORT_RCC RCC_APB2Periph_GPIOB
#define KEY_LS_PORT GPIOB
#define KEY_L1_PIN GPIO_Pin_12
#define KEY_L2_PIN GPIO_Pin_13
#define KEY_L3_PIN GPIO_Pin_14
#define KEY_L4_PIN GPIO_Pin_15

#define MKEY_NON_PRESSED 0x0000
#define MKEY1_PRESSED 0x0001
#define MKEY2_PRESSED 0x0002
#define MKEY3_PRESSED 0x0004
#define MKEY4_PRESSED 0x0008
#define MKEY5_PRESSED 0x0010
#define MKEY6_PRESSED 0x0020
#define MKEY7_PRESSED 0x0040
#define MKEY8_PRESSED 0x0080
#define MKEY9_PRESSED 0x0100
#define MKEY10_PRESSED 0x0200
#define MKEY11_PRESSED 0x0400
#define MKEY12_PRESSED 0x0800
#define MKEY13_PRESSED 0x1000
#define MKEY14_PRESSED 0x2000
#define MKEY15_PRESSED 0x4000
#define MKEY16_PRESSED 0x8000

void MKEY_init(void);

u16 MKEY_keyScan(void);

#endif

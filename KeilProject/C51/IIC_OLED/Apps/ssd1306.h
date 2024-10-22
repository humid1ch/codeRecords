#ifndef __SSD1306_H__
#define __SSD1306_H__

#include "util.h"

// OLED: SSD1306的地址
#define SSD1306_ADDR 0x78
#define WRITE_MODE 0X00

#define WRITE_CMD_CMD 0x00
#define WRITE_DATA_CMD 0x40

void SSD1306_init();
// 写命令
u8 SSD1306_writeCmd(u8 u8cmd);
// 写数据
u8 SSD1306_writeData(u8 u8data);

u8 SSD1306_writeDataBegin();

void SSD1306_writeDataOver();

u8 SSD1306_writeData_ECC(u8 u8data);

#endif

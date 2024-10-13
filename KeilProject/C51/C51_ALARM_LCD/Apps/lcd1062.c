#include "reg52.h"
#include "lcd1602.h"

sbit RW = P2 ^ 5; // 读写选择 引脚, 高电平读数据, 低电平可向LCD写数据
sbit RS = P2 ^ 6; // 数据/命令选择 引脚, 低电平 用于传输设置工作方式的命令, 高电平 用于传输需要显示的字符数据, 8位
sbit EN = P2 ^ 7; // 使能 引脚, 拉高可将 D0 ~ D7 引脚的数据写入LCD中, 反之 读取出来

static void _LCD1602_WriteData(u8 u8data);

// 清屏
void LCD1602_Init() {
    u8 i = 0;
    RW = 0; // 写
    EN = 0;

    RS = 0;                        // 写 命令
    _LCD1602_WriteData(CLEAR_CMD); // 清屏
    delayMs();
    _LCD1602_WriteData(ADDR_SET | 0x00);
    delayMs();

    // 模式设置 写入数据时 光标左移 显示器不动
    _LCD1602_WriteData(MODE_SET | CURSOR_R | DISPLAY_STOP);
    delayMs();

    // 开关设置 显示器开 光标关 闪烁关
    _LCD1602_WriteData(SWITCH_SET | DISPLAY_ON | CURSOR_ON | BLINK_OFF);
    delayMs();

    // 功能设置 数据总线宽度8 显示2行 高分辨率
    _LCD1602_WriteData(FUNC_SET | DATA_LINE_8BIT | TWO_LINE | CHAR_HIGH);
    delayMs();
}

void LCD1602_Clear() {
    RW = 0; // 写
    EN = 0;
    RS = 0;                        // 写 命令
    _LCD1602_WriteData(CLEAR_CMD); // 清屏
    delayMs();
}

void LCD1602_Dispaly(u8 u8addr, u8 u8data) {
    RW = 0; // 写
    EN = 0;

    RS = 0; // 写指令, 选址
    _LCD1602_WriteData(ADDR_SET | u8addr);
    delayMs();

    RS = 1;
    _LCD1602_WriteData(u8data);
    delayMs();
}

void LCD1602_FirstLine(const char* str) {
    char* cur = str;
    RW = 0; // 写
    EN = 0;

    // 光标复位到第一行第一位
    RS = 0;
    _LCD1602_WriteData(ADDR_SET | 0x00);
    delayMs();

    RS = 1;
    while (*cur != 0) {
        _LCD1602_WriteData(*cur);
        delayMs();
        cur++;
    }
}

void LCD1602_SecondLine(const char* str) {
    char* cur = str;
    RW = 0; // 写
    EN = 0;

    // 光标复位到第二行第一位
    RS = 0;
    _LCD1602_WriteData(ADDR_SET | 0x40);
    delayMs();

    RS = 1;
    while (*cur != 0) {
        _LCD1602_WriteData(*cur);
        delayMs();
        cur++;
    }
}

static void _LCD1602_WriteData(u8 u8data) {
    LCD_DATA = u8data;

    EN = 1;
    delay10Us();
    EN = 0;
    delay10Us();
}

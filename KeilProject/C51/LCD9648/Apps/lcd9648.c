#include "reg52.h"
#include "lcd9648.h"

//管脚定义
sbit CS0 = P0 ^ 3; // 片选
sbit RST = P0 ^ 2; // 复位
sbit RS = P0 ^ 4;  // 数据命令选择端
sbit SCL = P0 ^ 6; // SPI时钟端
sbit SDA = P0 ^ 5; // SPI数据端

void LCD9648_Init() {
    // 硬件复位
    RST = 0; // 复位拉低
    delay10Us();
    RST = 1; // 复位拉高
    delay10Us();

    CS0 = 1; // 片选(使能)拉高, 空闲
    SCL = 0; // SPI时钟端拉低, 空闲

    LCD9648_WriteCmd(0xE2); // 软件复位
    LCD9648_WriteCmd(0xC8); // 扫描方向, 0xC8从右到左, 0xC0从左到右
    LCD9648_WriteCmd(0xA0); // 段选方向?(决定了内存数据在, 显示器像素点的映射) 0xA0正常方向, 0xA1反方向
    LCD9648_WriteCmd(0x2F); // 0x2F命令, 启动 LCD9648, 不一定显示内容

    LCD9648_WriteCmd(0x26); // 发送0x26 开始对比度设置
    LCD9648_WriteCmd(0x81); // 发送0x81 开启设置对比度值设置
    LCD9648_WriteCmd(0x10); // 设置具体对比度 0x10

    LCD9648_WriteCmd(0xAF); // 发送0xAF 显示屏开启显示内容
    LCD9648_WriteCmd(0x40); // 设置滚动行?
}
void LCD9648_Clear() {}
// 暂时不了解 LCD9648的 极性 和 相性
void LCD9648_WriteData(u8 u8data) {
    u8 i = 0;
    CS0 = 0; // 片选拉低
    RS = 0;  // 写数据

    SCL = 0; // 时钟拉低, 空闲(猜的)
    // MSB 先发高位
    for (i = 0; i < 8; i++) {
        SDA = u8data >> 7;
        u8data <<= 1;
        SCL = 1; // 时钟拉高, 发送
        delay10Us();
        SCL = 0; // 时钟拉低, 空闲
        delay10Us();
    }

    CS0 = 1; // 片选拉高
}

void LCD9648_WriteCmd(u8 u8cmd) {
    u8 i = 0;
    CS0 = 0; // 片选拉低
    RS = 1;  // 写命令

    SCL = 0; // 时钟拉低, 空闲(猜的)
    // MSB 先发高位
    for (i = 0; i < 8; i++) {
        SDA = u8cmd >> 7;
        u8cmd <<= 1;
        SCL = 1; // 时钟拉高, 发送
        delay10Us();
        SCL = 0; // 时钟拉低, 空闲
        delay10Us();
    }

    CS0 = 1; // 片选拉高
}

void LCD_SHOW_Char(u8 x, u8 y, char c);
void LCD_SHOW_String(u8 x, u8 y, u8 len, const char* str);
void LCD_SHOW_16HZ(u8 x, u8 y, const u8* cn_map);
void LCD_SHOW_Num(u8 x, u8 y, u32 num, u8 len, u8 mode);
void LCD_SHOW_Image(u8* p);

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
    RST = 1;
    delayNMs(10);
    RST = 0; // 复位拉低
    delayNMs(10);
    RST = 1; // 复位拉高
    delayNMs(10);

    CS0 = 1; // 片选(使能)拉高, 空闲
    SCL = 0; // SPI时钟端拉低, 空闲

    LCD9648_WriteCmd(0xE2); // 软件复位
    LCD9648_WriteCmd(0xC8); // 扫描方向, 0xC8从右到左, 0xC0从左到右
    LCD9648_WriteCmd(0xA0); // 段选方向?(决定了内存数据在, 显示器像素点的映射) 0xA0正常方向, 0xA1反方向
    LCD9648_WriteCmd(0x2F); // 0x2F命令, 启动 LCD9648, 不一定显示内容

    LCD9648_WriteCmd(0x26); // 发送0x26 开始对比度控制
    LCD9648_WriteCmd(0x81); // 发送0x81 开启设置对比度值设置
    LCD9648_WriteCmd(0x10); // 设置具体对比度 0x10

    LCD9648_WriteCmd(0xAF); // 发送0xAF 显示屏开启显示内容
}

void LCD9648_Clear() {
    u8 i = 0;
    u8 j = 0;
    CS0 = 1; // 空闲
    SCL = 0; // 空闲

    // 滚动方向
    for (i = 0; i < 9; i++) {
        LCD9648_WriteCmd(0x40);
        // 页选
        LCD9648_WriteCmd(0xB0 + i); // 0xB0 应该是首页地址

        // 列地址设置
        LCD9648_WriteCmd(0x10); // 列地址, 高位0001
        LCD9648_WriteCmd(0x00); // 列地址, 低位0000

        // 写入数据时, 下一个写入地址会自增
        for (j = 0; j < LCD_WIDTH; j++)
            LCD9648_WriteData(0x00);
    }
}

// 暂时不了解 LCD9648的 极性 和 相性
static void LCD9648_WriteData(u8 u8data) {
    u8 i = 0;

    SCL = 0; // 时钟拉低, 空闲
    delay10Us();

    CS0 = 0; // 片选拉低
    delay10Us();
    RS = 1; // 写数据
    // MSB 先发高位
    for (i = 0; i < 8; i++) {
        SDA = (u8data & 0x80) ? 1 : 0; // 取高位
        u8data <<= 1;                  // 去高位

        SCL = 1; // 时钟拉高, 发送
        delay10Us();
        SCL = 0; // 时钟拉低, 空闲
        delay10Us();
    }

    SCL = 1; // 时钟拉高, 发送
    delay10Us();
    SCL = 0; // 时钟拉低, 空闲
    delay10Us();
    CS0 = 1; // 片选拉高
}

static void LCD9648_WriteCmd(u8 u8cmd) {
    u8 i = 0;

    SCL = 0; // 时钟拉低, 空闲
    delay10Us();
    CS0 = 0; // 片选拉低
    delay10Us();
    RS = 0; // 写命令

    // MSB 先发高位
    for (i = 0; i < 8; i++) {
        SDA = (u8cmd & 0x80) ? 1 : 0; // 取高位
        u8cmd <<= 1;                  // 去高位

        SCL = 1; // 时钟拉高, 发送
        delay10Us();
        SCL = 0; // 时钟拉低, 空闲
        delay10Us();
    }

    SCL = 1; // 时钟拉高, 发送
    delay10Us();
    SCL = 0; // 时钟拉低, 空闲
    delay10Us();
    CS0 = 1; // 片选拉高
}

void LCD_SHOW_Char(u8 x, u8 y, char c) {}
void LCD_SHOW_String(u8 x, u8 y, u8 len, const char* str);
void LCD_SHOW_16HZ(u8 x, u8 y, const u8* cnMap);
void LCD_SHOW_Num(u8 x, u8 y, u32 num, u8 len, u8 mode);
void LCD_SHOW_Image(u8* imageMap) {
    u8 i = 0;
    u8 j = 0;

    // 滚动方向
    for (i = 0; i < 6; i++) {
        LCD9648_WriteCmd(0x40);
        // 页选
        LCD9648_WriteCmd(0xB0 + i); // 0xB0 应该是首页地址

        // 列地址设置
        LCD9648_WriteCmd(0x10); // 列地址, 高位0001
        LCD9648_WriteCmd(0x00); // 列地址, 低位0000

        // 写入数据时, 下一个写入地址会自增
        for (j = 0; j < LCD_WIDTH; j++) {
            LCD9648_WriteData((*imageMap));
            imageMap++;
        }
    }
}

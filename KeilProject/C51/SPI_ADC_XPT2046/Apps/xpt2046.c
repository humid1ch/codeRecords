#include "reg52.h"
#include "intrins.h"
#include "xpt2046.h"

// 管脚
sbit DOUT = P3 ^ 7; // SPI输出
sbit DIN = P3 ^ 4;  // SPI输入
sbit CLK = P3 ^ 6;  // SPI时钟
sbit CS = P3 ^ 5;   // SPI片选

//函数接口
static void _SPI_WriteByte(u8 u8data);
static u16 _SPI_ReadData(void);

void XPT2046_Init() {}

u16 XPT2046_GetValue(u8 u8cmd) {
    u8 i;
    u16 u16val = 0;

    CLK = 0;
    CS = 1;
    _nop_();

    CS = 0;
    _nop_(); // 开启XPT2046通信

    _SPI_WriteByte(u8cmd); // 主机发送指令

    //间隔一个时钟，以清除BUSY
    for (i = 0; i < 10; i++)
        ;
    CLK = 1;
    _nop_();
    CLK = 0;
    _nop_();

    u16val = _SPI_ReadData();

    CS = 1; //关闭XPT2046通信
    _nop_();

    return u16val;
}

static void _SPI_WriteByte(u8 u8data) {
    u8 i;

    CLK = 0;
    _nop_();

    // MSB
    for (i = 0; i < 8; i++) {
        DIN = u8data >> 7;
        u8data <<= 1;

        CLK = 1;
        _nop_();
        CLK = 0;
        _nop_();
    }
}

static u16 _SPI_ReadData() {
    u16 u16val = 0;
    u8 i;

    CLK = 0;
    _nop_();

    // MSB
    for (i = 0; i < 12; i++) {
        u16val |= DOUT;
        u16val <<= 1;

        CLK = 1;
        _nop_();
        CLK = 0;
        _nop_();
    }

    return u16val;
}

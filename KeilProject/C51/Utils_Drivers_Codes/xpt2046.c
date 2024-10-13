#include "reg52.h"
#include "intrins.h"
#include "xpt2046.h"

// 管脚
sbit DOUT = P3 ^ 7; // SPI 输出
sbit DIN = P3 ^ 4;  // SPI 输入
sbit CLK = P3 ^ 6;  // SPI 时钟
sbit CS = P3 ^ 5;   // SPI 片选, 低电平 使能

//函数接口
static void _SPI_WriteByte(u8 u8data);
static u16 _SPI_ReadData(void);

void XPT2046_Init() {}

// 根据 XPT2046获取数据地时序图
// 要先 DIN 发送 8位地指令(MSB), 然后清除BUSY
// 再 DOUT 接收 12位的数据, 此12位的数据即为 模数转换之后的数据
// 无论收 发, 均为上升沿执行, 且 极性为0, 说明相性为0
//
// 发送数据, 需要先发送 控制指令, 用于选择 使能 相关接口和相关模式
// 7 6  5  4  3    2       1   0
// S A2 A1 A0 MODE SER√DFR PD1 PD0
// S, 开始位, 为1 表示一个新的控制指令字节要进行发送, 为0 忽略后续内容
// A2 A1 A0, 通道选择位, 即 接口使能
// MODE, 分辨率选择, 为0 12位分辨率, 为1 8位分辨率
// SER√DFR, 单端或差分模式选择
// PD1 PD0, 功率模式选择位, 00 低功率(变换间低功率), 11 高功率(总处于供电模式)
u16 XPT2046_GetValue(u8 u8cmd) {
    u8 i;
    u16 u16val = 0;

    CLK = 0;
    CS = 1;
    _nop_();

    CS = 0; // 开启XPT2046通信
    _nop_();

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
        DIN = u8data >> 7; // 取高位
        u8data <<= 1;      // 去高位

        CLK = 1;
        _nop_();
        CLK = 0;
        _nop_();
    }
}

static u16 _SPI_ReadData() {
    u8 i;
    u16 u16val = 0;

    CLK = 0;
    _nop_();

    // MSB
    for (i = 0; i < 12; i++) {
        u16val |= DOUT;
        if (i < 11)
            u16val <<= 1;

        CLK = 1;
        _nop_();
        CLK = 0;
        _nop_();
    }

    return u16val;
}

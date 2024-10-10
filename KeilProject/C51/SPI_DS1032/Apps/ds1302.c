/*
    DS1302时钟, SPI协议通信
    无论读写, 都是LSB模式, 即 发送数据 或 读取数据 均 先从低位开始发送
*/
#include "ds1302.h"
#include "reg52.h"
#include "intrins.h"

// 管脚
sbit DS1302_RST = P3 ^ 5; // SPI复位或片选
sbit DS1302_CLK = P3 ^ 6; // SPI时钟
sbit DS1302_IO = P3 ^ 4;  // SPI数据

// 存储 DS1302 时间. BCD格式, 即4位bit表示10进制
// 秒 分 时 日 月 周 年
u8 gDS1302_Time[7] = {0x50, 0x03, 0x15, 0x10, 0x10, 0x04, 0x24};

// 秒分时日月周年的 控制寄存器写的指令
static const u8 ds1302_rtc_cmd[7] = {SECONDS_CTRL, MINITES_CTRL, HOURS_CTRL, DAYS_CTRL, MONTHES_CTRL, WEEKS_CTRL, YEARS_CTRL};

static void _ds1302_WriteByte(u8 u8cmd, u8 u8data);
static u8 _ds1302_ReadByte(u8 u8cmd);

void DS1302_Init() {
    u8 i;

    // 关闭DS1302的写保护: 发送0x8E选择写保护寄存器, 向选定寄存器写入0x00 关闭写保护
    _ds1302_WriteByte(WRITE_PRO_CTRL | REG_W_CTRL, 0x00);

    // 按顺序向 秒分时日月周年 寄存器 写入初始时间
    for (i = 0; i < 7; i++) {
        _ds1302_WriteByte(ds1302_rtc_cmd[i] | REG_W_CTRL, gDS1302_Time[i]);
    }

    // 开启DS1302的写保护: 发送0x8E选择写保护寄存器, 向选定寄存器写入0x80 关闭开启保护
    _ds1302_WriteByte(WRITE_PRO_CTRL | REG_W_CTRL, 0x80);
}

void DS1302_GetTime() {
    u8 i;

    // 按顺序从 秒0 分1 时2 日3 月4 周5 年6 寄存器 读取 基于初始时间的
    for (i = 0; i < 7; i++) {
        gDS1302_Time[i] = _ds1302_ReadByte(ds1302_rtc_cmd[i] | REG_R_CTRL);
    }
}

static void _ds1302_WriteByte(u8 u8cmd, u8 u8data) {
    u8 i;

    //SPI片选或复位
    DS1302_RST = 0;
    _nop_();

    DS1302_CLK = 0;
    _nop_();

    DS1302_RST = 1;
    _nop_();

    // 发送指令 选择寄存器
    for (i = 0; i < 8; i++) {
        DS1302_IO = u8cmd & 0x01; //LSB
        u8cmd >>= 1;

        DS1302_CLK = 1;
        _nop_();
        DS1302_CLK = 0;
        _nop_();
    }

    // 向IO口写入数据
    for (i = 0; i < 8; i++) {
        DS1302_IO = u8data & 0x01; //LSB
        u8data >>= 1;

        DS1302_CLK = 1;
        _nop_();
        DS1302_CLK = 0;
        _nop_();
    }

    DS1302_RST = 0;
    _nop_();
}

static u8 _ds1302_ReadByte(u8 u8cmd) {
    u8 i;
    u8 u8val = 0;
    u8 u8temp = 0;

    //SPI片选或复位
    DS1302_RST = 0;
    _nop_();

    DS1302_CLK = 0;
    _nop_();

    DS1302_RST = 1;
    _nop_();

    // 发送指令, 选择寄存器
    for (i = 0; i < 8; i++) {
        DS1302_IO = u8cmd & 0x01; // LSB
        u8cmd >>= 1;

        DS1302_CLK = 1;
        _nop_();

        if (i < 7) {
            DS1302_CLK = 0;
            _nop_();
        }
    }

    for (i = 0; i < 8; i++) {
        DS1302_CLK = 0;
        _nop_();

        u8temp = DS1302_IO; // LSB的, 先接收到的是低位

        u8val |= (u8temp << i);

        DS1302_CLK = 1;
        _nop_();
    }

    DS1302_RST = 0;
    _nop_();

    //我们的开发板P34有问题，外部没有上拉电阻，需要一个P34的上升沿才能读到数据
    DS1302_CLK = 1;
    _nop_();

    DS1302_IO = 0;
    _nop_();
    DS1302_IO = 1;
    _nop_();

    return u8val;
}
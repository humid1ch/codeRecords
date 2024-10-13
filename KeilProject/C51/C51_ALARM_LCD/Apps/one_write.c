#include "one_write.h"
#include "reg52.h"
#include "intrins.h"

#define ONEWRITE_RES_TIMEOUT 25

sbit DS18B20_DQ = P3 ^ 7; //定义单总线设备IO

static u8 _readBit();

// one-write初始化
void ONE_WRITE_Init() {}

// one-write写一个字节数据
void ONE_WRITE_WriteByte(u8 u8data) {
    u8 i = 0;
    u8 u8temp = 0;

    for (i = 0; i < 8; i++) {
        u8temp = u8data & 0x01;
        u8data >>= 1;

        if (u8temp) {
            DS18B20_DQ = 0;
            _nop_();
            _nop_();
            DS18B20_DQ = 1;

            delayN10Us(6);
        }
        else {
            DS18B20_DQ = 0;
            delayN10Us(6);
            DS18B20_DQ = 1;
            _nop_();
            _nop_();
        }
    }
}

// one-write读一个字节数据
u8 ONE_WRITE_ReadByte(void) {
    u8 i = 0, u8temp = 0, u8data = 0;
    for (i = 0; i < 8; i++) {
        u8temp = _readBit(); //每次读取一个bit,先读的是低位LSB
        u8data |= (u8temp << 7);
        if (i < 7)
            u8data >>= 1;
    }

    return u8data;
}

//one-write设备复位
u8 ONE_WRITE_Reset(void) {
    u8 u8timeout = 0;

    //主机拉低复位
    DS18B20_DQ = 0;
    delayN10Us(50); //主机发送复位脉冲500us

    DS18B20_DQ = 1; //主机释放DQ线，等待从机回应
    delayN10Us(2);  //主机发送复位脉冲500us

    //主机判断DQ是否被从机拉低，250us超时
    while (DS18B20_DQ) {
        u8timeout++;

        if (u8timeout > ONEWRITE_RES_TIMEOUT)
            return 1; //复位异常

        delay10Us();
    }

    //主机判断DQ是否被释放
    u8timeout = 0;
    while (!DS18B20_DQ) {
        u8timeout++;

        if (u8timeout > ONEWRITE_RES_TIMEOUT)
            return 1; //复位异常

        delay10Us();
    }

    return 0;
}

static u8 _readBit(void) {
    u8 u8temp = 0;

    DS18B20_DQ = 0;
    _nop_(); //一个机器周期
    _nop_();

    DS18B20_DQ = 1;
    _nop_(); //一个机器周期
    _nop_();

    if (DS18B20_DQ)
        u8temp = 1;
    else
        u8temp = 0;

    delayN10Us(6);

    return u8temp;
}
#include "reg52.h"
#include "intrins.h"

typedef unsigned char u8;
typedef unsigned int u16;

void delay10Us() {
    while (0)
        ;
}

void delayN10Us(unsigned int n) {
    while (--n)
        delay10Us();
}

void delayMs() {
    unsigned char data i, j;

    _nop_();
    i = 2;
    j = 199;
    do {
        while (--j)
            ;
    } while (--i);
}

void delayNMs(u16 n) {
    while (--n)
        delayMs();
}

// IIC 总线通信 (软件)
// 相关驱动
sbit SDA = P2 ^ 0; // 数据口
sbit SCL = P2 ^ 1; // 时钟口

void iicStart();
void iicStop();
u8 iicWaitAck();
void iicAck();
void iicNAck();
void iicWriteByte(u8 u8data);
u8 iicReadByte();
// 上面的驱动 均根据IIC传输层协议规定实现

// AT24C02读写驱动接口 (EEPROM)
#define ADDR_W 0xA0
#define ADDR_R 0xA1

void at24c02Init();

void at24c02WriteByte(u8 u8addr, u8 u8data);

u8 at24c02ReadByte(u8 u8addr);

void main() {}

// 在时钟高电平时, SDA下降沿表示 数据传输开始
void iicStart() {
    // 先将SCL拉高
    SCL = 1;

    // 再将 SDA拉高, 保证下面出现下降沿
    SDA = 1;
    delay10Us();
    SDA = 0;
    delay10Us();
}

// 在时钟高电平时, SDA上升沿表示 数据传输结束
void iicStop() {
    // 先将SCL拉高
    SCL = 1;

    // 再将 SDA拉低, 保证下面出现上升沿
    SDA = 0;
    delay10Us();
    SDA = 1;
    delay10Us();
}

u8 iicWaitAck() {
    //释放SDA
    SDA = 1;
    SCL = 1;
    delay10Us();

    //读取 从机的应答
    if (SDA) { // 从机应答NACK
        // 结束通信
        iicStop();
        return 0;
    }
    // 如果是ACK, 不用单独处理

    SCL = 0;
    delay10Us();

    return 1;
}

// SDA低电平 是ACK响应
void iicAck() {
    SCL = 0;
    delay10Us();

    SDA = 0;
    delay10Us();
    SCL = 1;
    delay10Us();
}

// SDA低电平 是NACK响应
void iicNAck() {
    SCL = 0;
    delay10Us();

    SDA = 1;
    delay10Us();
    SCL = 1;
    delay10Us();
}

void iicWriteByte(u8 u8data) {

    u8 i;
    SCL = 0;
    delay10Us();

    for (i = 0; i < 8; i++) {
        SDA = u8data >> 7; //MSB

        SCL = 1;
        delay10Us();
        SCL = 0;
        delay10Us();
        u8data <<= 1;
    }

    //释放SDA
    SDA = 1;
    SCL = 1;
    delay10Us();
}

u8 iicReadByte() {
    u8 recv = 0;
    u8 i;

    for (i = 0; i < 8; i++) {
        SCL = 1;
        delay10Us();

        recv = (recv << 1) | SDA ? 1 : 0;

        SCL = 0;
        delay10Us();
    }

    return recv;
}

// AT24C02读写驱动接口 (EEPROM)
void at24c02Init() {}

void at24c02WriteByte(u8 u8addr, u8 u8data) {
    // 开始
    iicStart();
    // 发送 从机地址 和 读写方向
    iicWriteByte(ADDR_W);
    // 等待从机响应
    if (!iicWaitAck()) // 返回值为 0, 则表示通信结束
        return;

    // 发送 内存地址
    iicWriteByte(u8addr);
    // 等待从机应答
    if (!iicWaitAck()) // 返回值为 0, 则表示通信结束
        return;

    // 发送 数据
    iicWriteByte(u8data);
    // 等待从机应答
    if (!iicWaitAck()) // 返回值为 0, 则表示通信结束
        return;

    delayNMs(10);
}

u8 at24c02ReadByte(u8 u8addr) {
    u8 readRes;

    // 开始 发送传输方向
    iicStart();
    iicWriteByte(ADDR_W);
    // 等待从机响应
    if (!iicWaitAck()) // 返回值为 0, 则表示通信结束
        return 0;

    // 发送 内存地址
    iicWriteByte(u8addr);
    // 等待从机应答
    if (!iicWaitAck()) // 返回值为 0, 则表示通信结束
        return 0;

    // 转换通信方向为 读
    iicStart();
    iicWriteByte(ADDR_R);
    // 等待从机响应
    if (!iicWaitAck()) // 返回值为 0, 则表示通信结束
        return 0;

    // 读取 从机发送的数据
    readRes = iicReadByte();
    // 对从机发送 NACK响应, 让其终止发送的通信
    iicNAck();
    // 终止通信
    iicStop();

    return readRes;
}
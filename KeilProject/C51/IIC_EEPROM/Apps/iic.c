#include "reg52.h"
#include "iic.h"

// IIC 总线通信 (软件)
// 相关驱动
sbit SDA = P2 ^ 0; // 数据口
sbit SCL = P2 ^ 1; // 时钟口

// 在时钟高电平时, SDA下降沿表示 数据传输开始
void iicStart() {
    // 让 SDA和SCL处于空闲状态
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
    char timeout = 0;

    // SDA = 1; // SDA被拉低, 表示收到ACK应答, 拉高表示收到NACK应答
    SCL = 1;
    delay10Us();

    // 读取从机应答
    while (SDA == 1) {
        timeout++;
        if (timeout > 200)
            return 1; // 超时, 或收到 NACK
    }
    // 如果是ACK, 不用单独处理

    SCL = 0;
    delay10Us();

    return 0;
}

// SDA低电平 是ACK响应
void iicAck() {
    SCL = 0;
    SDA = 1;
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
    char i = 0;
    SCL = 0;
    delay10Us();

    for (i = 0; i < 8; i++) {
        SDA = (u8data >> 7) & 0x01; //MSB

        SCL = 1;
        delay10Us();
        SCL = 0;
        delay10Us();
        u8data <<= 1;
    }

    //释放SDA
    SCL = 1;
    SDA = 1;
    delay10Us();
}

u8 iicReadByte() {
    u8 recv = 0;
    char i = 0;

    for (i = 0; i < 8; i++) {
        SCL = 1;
        delay10Us();

        recv |= SDA ? 0x01 : 0x00;
        if (i < 7)
            recv <<= 1;

        SCL = 0;
        delay10Us();
    }

    return recv;
}
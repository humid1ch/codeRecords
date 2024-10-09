#include "at24c02.h"
#include "iic.h"

// AT24C02读写驱动接口 (EEPROM)
void at24c02Init() {}

// 正常返回0, 异常返回1
u8 at24c02WriteByte(u8 u8addr, u8 u8data) {
    // 开始
    iicStart();
    // 发送 从机地址 和 读写方向
    iicWriteByte(ADDR_W);
    // 等待从机响应
    if (iicWaitAck()) // 返回值为 0, 则表示通信结束
        return 1;

    // 发送 内存地址
    iicWriteByte(u8addr);
    // 等待从机应答
    if (iicWaitAck()) // 返回值为 0, 则表示通信结束
        return 1;

    // 发送 数据
    iicWriteByte(u8data);
    // 等待从机应答
    if (iicWaitAck()) // 返回值为 0, 则表示通信结束
        return 1;

    delayNMs(15);

    return 0;
}

u8 at24c02ReadByte(u8 u8addr) {
    u8 readRes = 0;

    // 开始 发送传输方向
    iicStart();
    iicWriteByte(ADDR_W);
    // 等待从机响应
    if (iicWaitAck()) // 返回值为 0, 则表示通信结束
        return 0;

    // 发送 内存地址
    iicWriteByte(u8addr);
    // 等待从机应答
    if (iicWaitAck()) // 返回值为 0, 则表示通信结束
        return 0;

    // 转换通信方向为 读
    iicStart();
    iicWriteByte(ADDR_R);
    // 等待从机响应
    if (iicWaitAck()) // 返回值为 0, 则表示通信结束
        return 0;

    // 读取 从机发送的数据
    readRes = iicReadByte();
    // 对从机发送 NACK响应, 让其终止发送的通信
    iicNAck();
    // 终止通信
    iicStop();

    return readRes;
}
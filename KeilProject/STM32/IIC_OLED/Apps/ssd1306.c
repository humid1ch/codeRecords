#include "ssd1306.h"
#include "iic.h"

void SSD1306_init(void) {
    IIC_init();
}

u8 SSD1306_writeCmd(u8 u8cmd) {
    // 开始
    IIC_start();
    // 发送 从机地址 和 读写方向
    IIC_writeByte(SSD1306_ADDR | WRITE_MODE);
    // 等待从机响应
    if (IIC_waitAck()) // 返回值为 0, 则表示通信结束
        return 1;

    IIC_writeByte(WRITE_CMD_CMD);
    if (IIC_waitAck()) // 返回值为 0, 则表示通信结束
        return 1;

    // 发送 数据
    IIC_writeByte(u8cmd);
    // 等待从机应答
    if (IIC_waitAck()) // 返回值为 0, 则表示通信结束
        return 1;

    // 停止?
    IIC_stop();

    return 0;
}

// 通信建立
u8 SSD1306_writeDataBegin(void) {
    // 开始
    IIC_start();
    // 发送 从机地址 和 读写方向
    IIC_writeByte(SSD1306_ADDR | WRITE_MODE);
    // 等待从机响应
    if (IIC_waitAck()) // 返回值为 0, 则表示通信结束
        return 1;

    return 0;
}
// 通信结束
void SSD1306_writeDataOver(void) {
    IIC_stop();
}
// 结合 Begin 和 Over 两个函数, 实现发送完数据之后 再断开通信
u8 SSD1306_writeData(u8 u8data) {
    IIC_writeByte(WRITE_DATA_CMD);
    if (IIC_waitAck()) // 返回值为 0, 则表示通信结束
        return 1;

    // 发送 数据
    IIC_writeByte(u8data);
    // 等待从机应答
    if (IIC_waitAck()) // 返回值为 0, 则表示通信结束
        return 1;

    return 0;
}

// 每次都断开通信
u8 SSD1306_writeData_ECC(u8 u8data) {
    // 开始
    IIC_start();
    // 发送 从机地址 和 读写方向
    IIC_writeByte(SSD1306_ADDR | WRITE_MODE);
    // 等待从机响应
    if (IIC_waitAck()) // 返回值为 0, 则表示通信结束
        return 1;

    IIC_writeByte(WRITE_DATA_CMD);
    if (IIC_waitAck()) // 返回值为 0, 则表示通信结束
        return 1;

    // 发送 数据
    IIC_writeByte(u8data);
    // 等待从机应答
    if (IIC_waitAck()) // 返回值为 0, 则表示通信结束
        return 1;

    IIC_stop();

    return 0;
}

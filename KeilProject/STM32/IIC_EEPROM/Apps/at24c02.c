#include "at24c02.h"
#include "iic.h"
#include "stddef.h"

void AT24C02_init(void) {
    IIC_init();
}

u8 AT24C02_iriteByte(u8 addr, u8 u8data) {
    IIC_start();
    IIC_writeByte(AT24C02_CMD_WRITE);
    if (IIC_waitAck() == 0) {
        IIC_stop();
        return 1; // 等待应答失败或NACK
    }

    IIC_writeByte(addr);
    if (IIC_waitAck() == 0) {
        IIC_stop();
        return 1; // 等待应答失败或NACK
    }
    IIC_writeByte(u8data);
    IIC_waitAck();

    IIC_stop();
    delayMs(2);

    return 0;
}

u8 AT24C02_readByte(u8 addr, u8* u8pdata) {
    if (u8pdata == NULL)
        return 1;

    *u8pdata = 0xff;

    IIC_start();
    IIC_writeByte(AT24C02_CMD_WRITE);
    if (IIC_waitAck() == 0) {
        IIC_stop();
        return 1; // 等待应答失败或NACK
    }

    IIC_writeByte(addr);
    if (IIC_waitAck() == 0) {
        IIC_stop();
        return 1; // 等待应答失败或NACK
    }

    //改变方向
    IIC_start();
    IIC_writeByte(AT24C02_CMD_READ);
    if (IIC_waitAck() == 0) {
        IIC_stop();
        return 1; // 等待应答失败或NACK
    }

    *u8pdata = IIC_weadByte();
    IIC_sendNack();
    IIC_stop();

    return 0;
}

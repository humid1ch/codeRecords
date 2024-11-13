#include "one_write.h"

#define ONEWRITE_RES_TIMEOUT 25

static u8 _readBit(void);
static void _OW_output(void);
static void _OW_input(void);

// one-write初始化
void OW_init(void) {
    RCC_APB2PeriphClockCmd(DATA_IO_RCC, ENABLE);

    GPIO_InitTypeDef GPIO_initStruct;
    GPIO_initStruct.GPIO_Pin = DATA_IO_PIN;
    GPIO_initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DATA_IO_PORT, &GPIO_initStruct);

    DATA_IO = 1;
}

// one-write写一个字节数据
void OW_writeByte(u8 u8data) {
    _OW_output();
    for (u8 i = 0; i < 8; i++) {
        u8 u8temp = u8data & 0x01;
        u8data >>= 1;

        if (u8temp) {
            DATA_IO = 0;
            __NOP();
            __NOP();

            DATA_IO = 1;
            delayUs(60);
        }
        else {
            DATA_IO = 0;
            delayUs(60);

            DATA_IO = 1;
            __NOP();
            __NOP();
        }
    }
}

// one-write读一个字节数据
u8 OW_readByte(void) {
    u8 u8data = 0;

    for (u8 i = 0; i < 8; i++) {
        u8 u8temp = _readBit(); // 每次读取一个bit,先读的是低位LSB
        u8data |= (u8temp << 7);
        if (i < 7)
            u8data >>= 1;
    }

    return u8data;
}

//one-write设备复位
u8 OW_reset(void) {
    u8 u8timeout = 0;

    _OW_output();
    //主机拉低复位
    DATA_IO = 0;
    delayUs(500); //主机发送复位脉冲500us

    DATA_IO = 1; //主机释放DQ线，等待从机回应
    delayUs(20); //主机发送复位脉冲500us

    _OW_input();
    //主机判断DQ是否被从机拉低，250us超时
    while (DATA_IO_R) {
        u8timeout++;

        if (u8timeout > ONEWRITE_RES_TIMEOUT)
            return 1; //复位异常

        delayUs(10);
    }

    //主机判断DQ是否被释放
    u8timeout = 0;
    while (!DATA_IO_R) {
        u8timeout++;

        if (u8timeout > ONEWRITE_RES_TIMEOUT)
            return 1; //复位异常

        delayUs(10);
    }

    return 0;
}

static u8 _readBit(void) {
    u8 u8temp = 0;

    _OW_output(); // 输出

    DATA_IO = 0;
    __NOP(); //一个机器周期
    __NOP();

    DATA_IO = 1;
    __NOP(); //一个机器周期
    __NOP();

    _OW_input(); // 输入
    if (DATA_IO_R)
        u8temp = 1;
    else
        u8temp = 0;

    delayUs(60);

    return u8temp;
}

static void _OW_input(void) {
    GPIO_InitTypeDef GPIO_initStruct;
    GPIO_initStruct.GPIO_Pin = DATA_IO_PIN;
    GPIO_initStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DATA_IO_PORT, &GPIO_initStruct);
}

static void _OW_output(void) {
    GPIO_InitTypeDef GPIO_initStruct;
    GPIO_initStruct.GPIO_Pin = DATA_IO_PIN;
    GPIO_initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DATA_IO_PORT, &GPIO_initStruct);
}

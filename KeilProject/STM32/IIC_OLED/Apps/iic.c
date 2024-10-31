#include "iic.h"
#include "utils.h"

static void _SDA_input(void);
static void _SDA_output(void);

// 初始化 IIC 总线
void IIC_init(void) {
    GPIO_InitTypeDef GPIO_InitStruct; //定义GPIO初始化结构体变量

    RCC_APB2PeriphClockCmd(IIC_SCL_RCC, ENABLE); //使能GPIOC时钟
    RCC_APB2PeriphClockCmd(IIC_SDA_RCC, ENABLE); //使能GPIOC时钟

    GPIO_InitStruct.GPIO_Pin = IIC_SCL_PIN | IIC_SDA_PIN; //设置引脚号
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;         //设置模式为推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;        //设置速度为最高频率
    GPIO_Init(IIC_SCL_PORT, &GPIO_InitStruct);            //初始化GPIO

    IIC_SCL = 1;
    IIC_SDA = 1;
}

//IIC 开始信号
void IIC_start(void) {
    _SDA_output();

    IIC_SCL = 1;

    IIC_SDA = 1;
    delayUs(5);
    IIC_SDA = 0;
    delayUs(5);

    IIC_SCL = 0; //准备发送数据
}

//IIC 停止信号
void IIC_stop(void) {
    _SDA_output();

    IIC_SCL = 1;

    IIC_SDA = 0;
    delayUs(5);
    IIC_SDA = 1;
    delayUs(5);
}

//IIC 等待应答信号
//返回值：(ack)1，接收应答；0(nack)，未接收应答
u8 IIC_waitAck(void) {
    u8 timeout = 0;

    IIC_SDA = 1; // 释放SDA的使用权
    delayUs(1);

    _SDA_input();

    IIC_SCL = 1; // 释放SCL的使用权
    delayUs(5);

    // 等待SDA从高电平变为低电平
    while (IIC_SDA_R == 1) {
        timeout++;
        //超时
        if (timeout > 200) {
            timeout = 0;
            return 1;
        }
    }

    IIC_SCL = 0; //占用SCL的使用权
    delayUs(5);

    return 0;
}

//IIC 发送一个字节
void IIC_writeByte(u8 u8data) {
    _SDA_output();

    //发送一个字节数据
    u8 i;
    IIC_SCL = 0;
    delayUs(5);

    for (i = 0; i < 8; i++) {
        IIC_SDA = u8data >> 7; //MSB

        IIC_SCL = 1;
        delayUs(5);
        IIC_SCL = 0;
        delayUs(5);

        u8data <<= 1;
    }
}

//IIC 读取一个字节
u8 IIC_readByte(void) {
    _SDA_input();

    //读取一个字节数据，ack=1时，应答，ack=0时，不应答
    u8 recv = 0;
    u8 i;

    for (i = 0; i < 8; i++) {
        IIC_SCL = 1;
        delayUs(5);

        recv |= IIC_SDA ? 0x01 : 0x00;
        if (i < 7)
            recv <<= 1;

        IIC_SCL = 0;
        delayUs(5);
    }

    return recv;
}

//IIC 发送应答ACK
void IIC_sendAck(void) {
    _SDA_output();

    //产生一个应答信号
    IIC_SCL = 0;
    delayUs(1);

    IIC_SDA = 0;
    delayUs(5);
    IIC_SCL = 1;
    delayUs(5);
}

//IIC 发送非应答NACK
void IIC_sendNack(void) {
    _SDA_output();

    //产生一个N应答信号
    IIC_SCL = 0;
    delayUs(1);

    IIC_SDA = 1;
    delayUs(5);
    IIC_SCL = 1;
    delayUs(5);
}

// 修改 SDA引脚为 input模式
static void _SDA_input(void) {
    GPIO_InitTypeDef GPIO_InitStruct; //定义GPIO初始化结构体变量

    GPIO_InitStruct.GPIO_Pin = IIC_SDA_PIN;        //设置引脚号
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;     //设置模式为上拉输入
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //设置速度为最高频率
    GPIO_Init(IIC_SDA_PORT, &GPIO_InitStruct);     //初始化GPIO
}

// 修改 SDA引脚为 output模式
static void _SDA_output(void) {
    GPIO_InitTypeDef GPIO_InitStruct; //定义GPIO初始化结构体变量

    GPIO_InitStruct.GPIO_Pin = IIC_SDA_PIN;        //设置引脚号
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  //设置模式为推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //设置速度为最高频率
    GPIO_Init(IIC_SDA_PORT, &GPIO_InitStruct);     //初始化GPIO
}

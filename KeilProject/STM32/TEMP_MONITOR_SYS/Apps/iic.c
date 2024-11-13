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

// 参数I2C_EVENT：事件名
void WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT) {
    u32 Timeout = 0x5000;
    // 等待一段时间后退出
    while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS) {
        Timeout--;
        if (Timeout == 0)
            break;
    }
}

//向指定寄存器地址写入指定数据
void IIC_writeReg(u8 regAddress, u8 u8data) {
    I2C_GenerateSTART(I2C1, ENABLE);               // I2C启动时序
    WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT); // 等待程序产生EV5事件 代表启动时序已完成

    I2C_Send7bitAddress(I2C1, 0x78, I2C_Direction_Transmitter);  // 发送从机地址 发送数据
    WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); // 等待程序产生EV6事件中的发送事件 代表发送完成

    I2C_SendData(I2C1, regAddress);                      // 发送指定寄存器地址
    WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING); // 等待程序产生的EV8事件 代表移位寄存器已移位 可以继续放入数据

    I2C_SendData(I2C1, u8data);                         // 发送要传输的数据
    WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // 等待程序产生的EV8_2事件 代表数据以全部发送完毕且数据寄存器没有新数据

    I2C_GenerateSTOP(I2C1, ENABLE); // I2C停止时序
}

//从指定寄存器地址读一个字节数据
u8 IIC_readReg(u8 regAddress) {
    u8 resultData;

    I2C_GenerateSTART(I2C1, ENABLE);               // I2C启动时序
    WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT); // 等待程序产生EV5事件 代表启动时序已完成

    I2C_Send7bitAddress(I2C1, 0x78, I2C_Direction_Transmitter);  // 发送从机地址 发送数据
    WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); // 等待程序产生EV6事件中的发送事件 代表发送完成

    I2C_SendData(I2C1, regAddress);                     // 发送指定寄存器地址
    WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // 等待程序产生的EV8_2事件 代表数据以全部发送完毕且数据寄存器没有新数据

    I2C_GenerateSTART(I2C1, ENABLE);               // 重新产生I2C起始时序
    WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT); // 等待程序产生EV5事件 代表启动时序已完成

    I2C_Send7bitAddress(I2C1, 0x78, I2C_Direction_Receiver);  // 发送从机地址 接收数据
    WaitEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED); // 等待程序产生EV6事件中的接收事件 代表发送完成

    I2C_AcknowledgeConfig(I2C1, DISABLE); // 发送从机地址后就已经开始接收数据了，由于只需要接收一个数据，所以在数据完全接收到前先给非应答模式，避免接收到数据后立即就开始重新接收数据
    I2C_GenerateSTOP(I2C1, ENABLE);       // 发送停止时序 该时序以及非应答会在数据接收完成之后开始执行
    // 上方两句代码只在接收最后一个数据时要写 如果接收的不是最后一个数据即连续读，就不允许该两句，直接运行后两句 知道接收最后一个数据前再运行上方两句
    WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED); // 等待程序产生EV7事件 代表数据接收完毕
    resultData = I2C_ReceiveData(I2C1);              // 读取接收到的数据

    I2C_AcknowledgeConfig(I2C1, ENABLE); // 重新将ACK改为应答模式

    return resultData;
}

void IIC_initHard(void) {
    GPIO_InitTypeDef GPIO_InitStructure; // 配置端口用结构体
    I2C_InitTypeDef I2C_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE); // 开启I2C的时钟
    RCC_APB2PeriphClockCmd(IIC_SDA_RCC, ENABLE);         // 开启GPIO的时钟

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;          // 引脚模式 复用开漏输出模式
    GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN | IIC_SDA_PIN; // 引脚号
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        // 引脚速度 50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);                   // 开始配置端口模式

    //I2C配置用结构体
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;                               // 收到数据后给ACK应答
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // I2C作为从机设备时的自身地址：7位地址
    I2C_InitStructure.I2C_ClockSpeed = 400000;                                // 时钟速度 400KHz 高速
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;                        // 快速模式下占空比为16/9（此时为低速 没啥影响）
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;                                // I2C模式
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;                                 // I2C作为从机设备时的自身地址：0x00
    I2C_Init(I2C1, &I2C_InitStructure);

    I2C_Cmd(I2C1, ENABLE);
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

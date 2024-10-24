#include "led_dots.h"
#include "utils.h"

// 1 2 3 4 5 6 7 8 col
u8 gU8LedCols[8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};
// 1 2 3 4 5 6 7 8 row
u8 gU8LedRows[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

void LEDOTS_init(void) {
    // PA0~7 时钟
    RCC_APB2PeriphClockCmd(COLS_PORT_RCC, ENABLE);
    // PB3 4 关闭JTAG
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
    // PB 时钟
    RCC_APB2PeriphClockCmd(RCLK_PORT_RCC, ENABLE);

    GPIO_InitTypeDef GPIOA_initStruct;
    GPIOA_initStruct.GPIO_Pin = COLS_PIN;
    GPIOA_initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOA_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(COLS_PORT, &GPIOA_initStruct);

    GPIO_InitTypeDef GPIOB_initStruct;
    GPIOB_initStruct.GPIO_Pin = SER_PIN | RCLK_PIN | SCLK_PIN;
    GPIOB_initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOB_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RCLK_PORT, &GPIOB_initStruct);

    LEDOTS_writeByte(COLS_PORT, 0x00, WRITE_L8);
}

// 参数: GPIOx 引脚组, value 要写入引脚的8位值, HLBits 写入高八位还是第八位
void LEDOTS_writeByte(GPIO_TypeDef* GPIOx, u8 value, u8 HL8Bits) {
    // 读出当前的引脚 16位 输出状态
    u16 u16value = GPIO_ReadOutputData(GPIOx);
    if (HL8Bits == WRITE_H8) {
        u16value &= 0x00FF;     // 高八位置零
        u16value |= value << 8; // 高八位赋值
    }

    if (HL8Bits == WRITE_L8) {
        u16value &= 0xFF00; // 第八位置零
        u16value |= value;  // 第八位赋值
    }

    GPIO_Write(GPIOx, u16value);
}

// 参数 是需要经过 595 并行发送的数据
void HC595SendByte(u8 u8data) {
    u8 i = 0;
    // 将u8data从高位到低位 依次写入595的移位寄存器
    for (i = 0; i < 8; i++) {
        SER = u8data >> 7; // u8data最高位赋给SER引脚
        u8data <<= 1;      // 更新u8data的最高位

        // 控制SRCLK引脚, 将赋给SER的数据写入到移位寄存器中
        SCLK_UP_ONCE();
    }

    // 控制 rCLK引脚, 将移位寄存器数据拷贝到数据寄存器, 并 并行输出
    RCLK_UP_ONCE();
}
/*
595输出的数据 是控制LED点阵 每行的电平的
595中 数据寄存器 从高到低位 对应的引脚是			而 LED点阵中, 从第1->8行 对应引脚为
1-DPh										  1-DPa
2-DPg										  2-DPb
3-DPf										  3-DPc
4-DPe										  4-DPd
5-DPd										  5-DPe
6-DPc										  6-DPf
7-DPb										  7-DPg
8-DPa										  8-DPh

所以, 若需要实现 以 输入数据从高位到低位 控制 LED点阵的第1 到 第8行
就需要, 先向595的移位寄存器中写入数据的高位数据
*/

void LEDOTS_rowCirculate(void) {
    LEDOTS_writeByte(COLS_PORT, 0x00, WRITE_L8);
    LEDOTS_writeByte(COLS_PORT, 0xFF, WRITE_L8);
    for (char i = 0; i < 8; i++) {
        HC595SendByte(gU8LedRows[i]);
        delayMs(100);
    }

    for (char i = 7; i >= 0; i--) {
        HC595SendByte(gU8LedRows[i]);
        delayMs(100);
    }
}

void LEDOTS_colCirculate(void) {
    LEDOTS_writeByte(COLS_PORT, 0x00, WRITE_L8);
    HC595SendByte(0x00);
    for (char i = 0; i < 8; i++) {
        LEDOTS_writeByte(COLS_PORT, gU8LedCols[i], WRITE_L8);
        delayMs(100);
    }

    for (char i = 7; i >= 0; i--) {
        LEDOTS_writeByte(COLS_PORT, gU8LedCols[i], WRITE_L8);
        delayMs(100);
    }
}

void LEDOTS_display(u8* u8Rows) {
    HC595SendByte(0x00); //消影
    for (u8 i = 0; i < 8; i++) {
        HC595SendByte(u8Rows[i]);                             //LED行数据
        LEDOTS_writeByte(COLS_PORT, gU8LedCols[i], WRITE_L8); //LED列数据
        delayMs(2);                                           //保持时间
        HC595SendByte(0x00);                                  //消影
    }
}

// 为能够 无缝循环显示, 需要将屏幕首帧需要显示的画面, 在结尾放一下
// 即 数组中首8个元素, 需要在数组尾再放一遍
void LEDOTS_leftScroll(const u8* array, const u8 size) {
    u8 ledOffset = 0; // led显示数数组的偏移量
    while (ledOffset < size - 7) {
        for (u8 i = 0; i < 5; i++) {
            LEDOTS_display(array + ledOffset);
        }
        ledOffset++;
    }
}

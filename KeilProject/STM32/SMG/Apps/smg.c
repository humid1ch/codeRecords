#include "smg.h"
#include "utils.h"

void SMG1(void);
void SMG2(void);
void SMG3(void);
void SMG4(void);
void SMG5(void);
void SMG6(void);
void SMG7(void);
void SMG8(void);

// 数码管选择函数 数组
smgSelFunc smgFuncs[] = {SMG1, SMG2, SMG3, SMG4, SMG5, SMG6, SMG7, SMG8};

// 板子使用 共阴数码管, 各引脚给高电平点亮边
// a b c d e f g dp  低->高
// dp g f e d c b a
// 0 1 2 3 4 5 6 7 8 9 A B C D E F . 空
u8 smgNum[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x80, 0x00};

void SMG_init(void) {
    // 数码管位选 GPIOB RCC
    RCC_APB2PeriphClockCmd(SMG_BIT_RCC, ENABLE);

    // 数码管位选引脚IO功能开启
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    // 关闭引脚的 JTAG功能
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    // 数码管段选 GPIOA RCC
    RCC_APB2PeriphClockCmd(SMG_SEG_RCC, ENABLE);

    GPIO_InitTypeDef SMG_SEG_initStruct;
    SMG_SEG_initStruct.GPIO_Pin = SMG_SEG0_PIN | SMG_SEG1_PIN | SMG_SEG2_PIN | SMG_SEG3_PIN | SMG_SEG4_PIN | SMG_SEG5_PIN | SMG_SEG6_PIN | SMG_SEG7_PIN;
    SMG_SEG_initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    SMG_SEG_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SMG_SEG_PORT, &SMG_SEG_initStruct);

    GPIO_InitTypeDef SMG_BIT_initStruct;
    SMG_BIT_initStruct.GPIO_Pin = SMG_BIT0_PIN | SMG_BIT1_PIN | SMG_BIT2_PIN;
    SMG_BIT_initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    SMG_BIT_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SMG_BIT_PORT, &SMG_BIT_initStruct);
}

// 参数: GPIOx 引脚组, value 要写入引脚的8位值, HLBits 写入高八位还是第八位
void SMG_writeByte(GPIO_TypeDef* GPIOx, u8 value, u8 HL8Bits) {
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

void SMG_display(const u8* smgCode) {
    for (u8 i = 0; i < 8; i++) {
        smgFuncs[i]();
        SMG_writeByte(SMG_SEG_PORT, smgNum[smgCode[i]], WRITE_L8);
        delayMs(2);
        SMG_writeByte(SMG_SEG_PORT, 0x00, WRITE_L8);
    }
}

// 8个 1位数码管选择函数
void SMG1(void) {
    SMG_BIT2 = 1;
    SMG_BIT1 = 1;
    SMG_BIT0 = 1;
}
void SMG2(void) {
    SMG_BIT2 = 1;
    SMG_BIT1 = 1;
    SMG_BIT0 = 0;
}
void SMG3(void) {
    SMG_BIT2 = 1;
    SMG_BIT1 = 0;
    SMG_BIT0 = 1;
}
void SMG4(void) {
    SMG_BIT2 = 1;
    SMG_BIT1 = 0;
    SMG_BIT0 = 0;
}
void SMG5(void) {
    SMG_BIT2 = 0;
    SMG_BIT1 = 1;
    SMG_BIT0 = 1;
}
void SMG6(void) {
    SMG_BIT2 = 0;
    SMG_BIT1 = 1;
    SMG_BIT0 = 0;
}
void SMG7(void) {
    SMG_BIT2 = 0;
    SMG_BIT1 = 0;
    SMG_BIT0 = 1;
}
void SMG8(void) {
    SMG_BIT2 = 0;
    SMG_BIT1 = 0;
    SMG_BIT0 = 0;
}

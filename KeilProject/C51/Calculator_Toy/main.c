/*
正整数计算器:
具体功能:
1. 矩阵按键, 实现数据、计算(=)、重新输入(全清)、删除(<-)的输入
    7 8 9 <-
    4 5 6 全清
    1 2 3 
      0   =
    
2. 独立按键, 实现 + - * / 输入
    +(K1) -(K2) *(K3) /(K4)

3. 先输入运算符左边数据, 最高四位 
   再输入 运算符
   然后输入运算符右边数据, 最高四位

4. 数据仅在输入时显示
    左边四个 1位数码管, 用于显示运算符左值
    右边四个 1为数码管, 用于显示运算符右值
    运算符 不在数码管进行显示, 由LED表示
    +, LED1 亮
    -, LED2 亮
    *, LED6 亮
    /, LED7 亮
    =, LED8 亮
*/

#include <reg52.h>
#include <intrins.h>

typedef unsigned char u8;
typedef unsigned int u16;

void delayMs() {
    u8 i, j;

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


// LED 灯
#define LED P2
#define LED1_ON 0xFE
#define LED2_ON 0xFD
#define LED6_ON 0xDF
#define LED7_ON 0xBF
#define LED8_0N 0x7F

// 8位数码管引脚
#define SMG P0

sbit SMG_22 = P2 ^ 2;
sbit SMG_23 = P2 ^ 3;
sbit SMG_24 = P2 ^ 4;

// 矩阵按键
#define KEY_PORT P1
#define KEY_UNPRESS 0
#define KEY1_PRESSED 1
#define KEY2_PRESSED 2
#define KEY3_PRESSED 3
#define KEY4_PRESSED 4
#define KEY5_PRESSED 5
#define KEY6_PRESSED 6
#define KEY7_PRESSED 7
#define KEY8_PRESSED 8
#define KEY9_PRESSED 9
#define KEY10_PRESSED 10
#define KEY11_PRESSED 11
#define KEY12_PRESSED 12
#define KEY13_PRESSED 13
#define KEY14_PRESSED 14
#define KEY15_PRESSED 15
#define KEY16_PRESSED 16

u8 keyMaps[][4] = {{KEY1_PRESSED, KEY2_PRESSED, KEY3_PRESSED, KEY4_PRESSED},
                   {KEY5_PRESSED, KEY6_PRESSED, KEY7_PRESSED, KEY8_PRESSED},
                   {KEY9_PRESSED, KEY10_PRESSED, KEY11_PRESSED, KEY12_PRESSED},
                   {KEY13_PRESSED, KEY14_PRESSED, KEY15_PRESSED, KEY16_PRESSED}};

u8 matrixKeyScan(); // 4x4矩阵按键 翻转扫描

// 独立按键
#define NON_KEY_PUSH 0
#define S_KEY1_PRESSED 1
#define S_KEY2_PRESSED 2
#define S_KEY3_PRESSED 3
#define S_KEY4_PRESSED 4

sbit S_KEY1 = P3 ^ 1;
sbit S_KEY2 = P3 ^ 0;
sbit S_KEY3 = P3 ^ 2;
sbit S_KEY4 = P3 ^ 3;

u8 singleKeyScan(); // 独立按键扫描

typedef void (*smgSelFunc)(); // 数码管选择函数 指针
// 8个 1位数码管选择函数, 位选
void smg1();
void smg2();
void smg3();
void smg4();
void smg5();
void smg6();
void smg7();
void smg8();
// 数码管选择函数 数组
smgSelFunc smgFuncs[] = {smg1, smg2, smg3, smg4, smg5, smg6, smg7, smg8};
// 数码管显示值 数组, 十六进制 0~F
u8 smgNum[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00};

// 显示值
void valueDisplay();
// 显示结果
void resultDisplay();

// NON
#define NON 0
// +
#define ADD 1
// -
#define SUB 2
// *
#define MULTI 3
// /
#define DIV 4

u8 leftValueArr[4] = {0}; // 左值数组
char leftIndex = -1;

u8 rightValueArr[4] = {0}; // 右值数组
char rightIndex = -1;

u8 operators = 0;       // 操作符

u8 resultArr[10] = {0}; // 结果数组
char resultIndex = -1;

// 输入计算数
void pushValue(u8 num);
// 根据左值 右值和运算符进行计算
void claculateRes();
// 回退
void backspace();

void main() {
    u8 matrixKeyRes = KEY_UNPRESS; // 记录矩阵按键
    u8 singleKeyRes = KEY_UNPRESS; // 记录独立按键

    while (1) {
        if (resultIndex != -1) { // 有计算结果
            resultDisplay();
        }
        else { // 无计算结果
            valueDisplay();
        }

        matrixKeyRes = matrixKeyScan(); // 矩阵按键扫描
        singleKeyRes = singleKeyScan(); // 独立按键扫描

        switch (matrixKeyRes) {
        case KEY1_PRESSED: // S1  7
            pushValue(7);
            break;
        case KEY2_PRESSED: // S2  8
            pushValue(8);
            break;
        case KEY3_PRESSED: // S3  9
            pushValue(9);
            break;
        case KEY5_PRESSED: // S5  4
            pushValue(4);
            break;
        case KEY6_PRESSED: // S6  5
            pushValue(5);
            break;
        case KEY7_PRESSED: // S7  6
            pushValue(6);
            break;
        case KEY9_PRESSED: // S9  1
            pushValue(1);
            break;
        case KEY10_PRESSED: // S10  2
            pushValue(2);
            break;
        case KEY11_PRESSED: // S11  3
            pushValue(3);
            break;
        case KEY14_PRESSED: // S14  0
            pushValue(0);
            break;
        case KEY4_PRESSED: // S4 回退
            backspace();
            break;
        case KEY8_PRESSED:    // S8 清空
            leftIndex = -1;   // 清空左值
            rightIndex = -1;  // 清空右值
            resultIndex = -1; // 清空结果
            operators = 0;    // 清除运算符
            LED = 0xFF;       // 熄灭LED
            break;
        case KEY16_PRESSED:  // S16 确认
            LED = LED8_0N;   // 亮 = 对应的灯
            claculateRes();  // 计算结果
            leftIndex = -1;  // 试图确认之后, 清空左值
            rightIndex = -1; // 试图确认之后, 清空右值
            operators = 0;   // 清除运算符
            break;

        default:
            break;
        }

        if (rightIndex == -1) {     // 右值存在, 不再改变运算符 (也可以不做限制)
            switch (singleKeyRes) {
            case S_KEY1_PRESSED: // K1 +
                operators = ADD;
                LED = LED1_ON;
                break;
            case S_KEY2_PRESSED: // K2 -
                operators = SUB;
                LED = LED2_ON;
                break;
            case S_KEY3_PRESSED: // K3 *
                operators = MULTI;
                LED = LED6_ON;
                break;
            case S_KEY4_PRESSED: // K4 /
                operators = DIV;
                LED = LED7_ON;
                break;

            default:
                break;
            }
        }
    }
}

// 矩阵按键扫描
u8 matrixKeyScan() {
    u8 keyRes = KEY_UNPRESS;

    u8 row = -1; // 行
    u8 col = -1; // 列
    // 列 扫描, P13为左手第一列, P17为第一行
    // 即 P1的八位中, 低四位表示列, 高四位表示行

    KEY_PORT = 0x0F;
    if (KEY_PORT != 0x0F) {
        delayNMs(10);
        if (KEY_PORT != 0x0F) {
            switch (KEY_PORT) {
            case 0x07:
                col = 0;
                break;
            case 0x0B:
                col = 1;
                break;
            case 0x0D:
                col = 2;
                break;
            case 0x0E:
                col = 3;
                break;

            default:
                break;
            }

            KEY_PORT = 0xF0;
            switch (KEY_PORT) {
            case 0x70:
                row = 0;
                break;
            case 0xB0:
                row = 1;
                break;
            case 0xD0:
                row = 2;
                break;
            case 0xE0:
                row = 3;
                break;

            default:
                break;
            }

            while (KEY_PORT != 0xF0)
                ;
        }

        keyRes = keyMaps[row][col];
    }

    return keyRes;
}
// 独立按键扫描
u8 singleKeyScan() {
    u8 keyRes = 0;

    if (S_KEY1 == 0 || S_KEY2 == 0 || S_KEY3 == 0 || S_KEY4 == 0) {
        delayNMs(2); // 消抖, 确认是否被按下
        if (S_KEY1 == 0)
            keyRes = S_KEY1_PRESSED;

        if (S_KEY2 == 0)
            keyRes = S_KEY2_PRESSED;

        if (S_KEY3 == 0)
            keyRes = S_KEY3_PRESSED;

        if (S_KEY4 == 0)
            keyRes = S_KEY4_PRESSED;
    }

    while (S_KEY1 == 0 || S_KEY2 == 0 || S_KEY3 == 0 || S_KEY4 == 0)
        ;

    // 返回keyRes
    return keyRes;
}

// 显示值
void valueDisplay() {
    char i = 0; // valueIndex是char类型的, 防止发生整型提升 所以i使用char
    // 左值显示
    for (i = 0; i <= leftIndex; i++) {
        smgFuncs[i]();                 // 选择数码管
        SMG = smgNum[leftValueArr[i]]; // 显示数值
        delayMs();
        SMG = smgNum[16];
    }
    // 右值显示
    for (i = 0; i <= rightIndex; i++) {
        smgFuncs[i + 4]();              // 选择数码管
        SMG = smgNum[rightValueArr[i]]; // 显示数值
        delayMs();
        SMG = smgNum[16];
    }
}

// 显示结果
void resultDisplay() {
    char i = 0; // valueIndex是char类型的, 防止发生整型提升 所以i使用char
    // 结果显示
    for (i = 0; i <= resultIndex; i++) {
        smgFuncs[i]();              // 选择数码管
        SMG = smgNum[resultArr[i]]; // 显示数值
        delayMs();
        SMG = smgNum[16];
    }
}

// 输入计算数
void pushValue(u8 num) {
    if (resultIndex != -1) // 如果上次计算了, 按下新数 就把结果清空
        resultIndex = -1;

    if (operators) { // 如果已经输入了运算符, 就往右值数组添加
        if (rightIndex < 3)
            rightValueArr[++rightIndex] = num;
    }
    else { // 如果没有输入运算符, 就往左值添加
        if (leftIndex < 3)
            leftValueArr[++leftIndex] = num;
    }
}
// 计算结果
void claculateRes() {
    // 先将数值数组 转换为数值
    long leftValue = 0;
    long rightValue = 0;
    // 这里遇到一个问题
    // 如果使用 int 或 unsigned int 接收leftValue和rightValue
    // 是可以成功接收输入的计算数据的, 因为左值和右值最多只有4位
    // 
    // 但是, 在后面的计算中会发生错误
    //  C语言中, 如果小整型进行计算, 如果计算结果会从小类型溢出, 那么 计算结果 会 临时自动提升到 能够不溢出的类型, 
    //  然后可以用任意类型进行接收, 接受结果 只会被接收数据的变量类型影响
    // 
    //  但, 在C51中, 好像并不是这样(也或许只是8051这样处理)
    //  C51中, 即使 小整型数据的 计算结果会溢出, 计算结果也并不会提升类型, 只会被限制在原本的最大类型中
    //      举个例子, int * int 计算结果的类型就只会是int, 范围就只是 -32,768 到 32,767
    //       256 * 128的结果是 32768, 如果在C51中以 int类型计算, 结果就会是0, 因为 32768 的二进制是 1000 0000 0000 0000, 首位是符号位 刚好被截断
    //       而 255 * 128的结果是 32640, 在C51中就能够完整计算出来
    //      同理, unsigned int * unsigned int计算结果的类型就只会是unsigned int, 范围就只是 0~65535
    //       256 * 256的结果是65536, 在C51中以 unsigned int类型计算, 结果是0, 因为 65536的二进制是 1 0000 0000 0000 0000, 刚好比4字节多1位
    //       256 * 255的结果是65280, 在C51中以 unsigned int类型计算, 结果能够完整计算出来

    // 这个问题让我想到另外一个问题: C51中存在整型提升吗?
    //  C语言 整型提升, 不足4字节的整型变量进行计算时, 会将原数据提升到4字节然后进行计算
    // 	也不好调试

    long result = 0;
    long resultTmp = 0;

    char i = 0;
    char j = 0;

    // 计算之前, 先清除结果 防止上次计算结果未清除
    resultIndex = -1;

    // 转左值
    for (i = 0; i <= leftIndex; i++) {
        leftValue *= 10;
        leftValue += leftValueArr[i];
    }
    // 转右值
    for (i = 0; i <= rightIndex; i++) {
        rightValue *= 10;
        rightValue += rightValueArr[i];
    }

    switch (operators) {
    case ADD:
        result = leftValue + rightValue;
        break;
    case SUB:
        result = leftValue - rightValue;
        break;
    case MULTI:
        result = leftValue * rightValue;
        break;
    case DIV:
        if (rightValue != 0)
            result = leftValue / rightValue;
        else
            result = 0;
        break;
    case NON:
        result = leftValue; // 没有操作符结果就等于左值
        break;

    default:
        break;
    }

    // 非正整数 懒得处理了 简单的处理为0
    if (result <= 0) {
        result = 0;
        resultArr[++resultIndex] = 0;
        
        return;
    }

    // 将计算结果, 存到resultArr中
    while (result) {
        resultTmp = (result % 10); // 求个位
        result /= 10;              // 去个位

        resultArr[++resultIndex] = resultTmp; // 填数组
    }

    // 翻转数组
    i = 0;
    j = resultIndex;
    while (i < j) {
        resultArr[i] = resultArr[i] + resultArr[j];
        resultArr[j] = resultArr[i] - resultArr[j];
        resultArr[i] = resultArr[i] - resultArr[j];
        i++;
        j--;
    }
    // 结果数组计算完成
}
// 回退
void backspace() {
    if (operators) { // 如果已经输入了运算符, 就回退右值
        if (rightIndex >= 0)
            rightIndex--;
    }
    else { // 如果没有输入运算符, 就回退左值
        if (leftIndex >= 0)
            leftIndex--;
    }
}

// 8个 1位数码管选择函数
void smg1() {
    SMG_24 = 1;
    SMG_23 = 1;
    SMG_22 = 1;
}

void smg2() {
    SMG_24 = 1;
    SMG_23 = 1;
    SMG_22 = 0;
}

void smg3() {
    SMG_24 = 1;
    SMG_23 = 0;
    SMG_22 = 1;
}

void smg4() {
    SMG_24 = 1;
    SMG_23 = 0;
    SMG_22 = 0;
}

void smg5() {
    SMG_24 = 0;
    SMG_23 = 1;
    SMG_22 = 1;
}

void smg6() {
    SMG_24 = 0;
    SMG_23 = 1;
    SMG_22 = 0;
}

void smg7() {
    SMG_24 = 0;
    SMG_23 = 0;
    SMG_22 = 1;
}

void smg8() {
    SMG_24 = 0;
    SMG_23 = 0;
    SMG_22 = 0;
}
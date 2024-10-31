#include "lcd9648.h"
#include "font.h"

static void _LCD9648_writeData(u8 u8data);
static void _LCD9648_writeCmd(u8 u8cmd);
static void _LCD9648_showChar(u8 x, u8 y, char c);

void LCD9648_init(void) {
    // 先初始化GPIO
    // 时钟使能
    RCC_APB2PeriphClockCmd(LCD9648_RST_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(LCD9648_RS_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(LCD9648_CS0_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(LCD9648_SDA_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(LCD9648_SCL_RCC, ENABLE);

    // 引脚初始化
    GPIO_InitTypeDef RST_initStruct;
    RST_initStruct.GPIO_Pin = LCD9648_RST_PIN;
    RST_initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    RST_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD9648_RST_PORT, &RST_initStruct);

    GPIO_InitTypeDef RS_initStruct;
    RS_initStruct.GPIO_Pin = LCD9648_RS_PIN;
    RS_initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    RS_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD9648_RS_PORT, &RS_initStruct);

    GPIO_InitTypeDef CS0_initStruct;
    CS0_initStruct.GPIO_Pin = LCD9648_CS0_PIN;
    CS0_initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    CS0_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD9648_CS0_PORT, &CS0_initStruct);

    GPIO_InitTypeDef SCL_initStruct;
    SCL_initStruct.GPIO_Pin = LCD9648_SCL_PIN;
    SCL_initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    SCL_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD9648_SCL_PORT, &SCL_initStruct);

    GPIO_InitTypeDef SDA_initStruct;
    SDA_initStruct.GPIO_Pin = LCD9648_SDA_PIN;
    SDA_initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    SDA_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD9648_SDA_PORT, &SDA_initStruct);

    // 硬件复位
    LCD9648_RST = 1;
    delayMs(10);
    LCD9648_RST = 0; // 复位拉低
    delayMs(10);
    LCD9648_RST = 1; // 复位拉高
    delayMs(10);

    LCD9648_CS0 = 1; // 片选(使能)拉高, 空闲
    LCD9648_SCL = 0; // SPI时钟端拉低, 空闲

    _LCD9648_writeCmd(0xE2); // 软件复位
    _LCD9648_writeCmd(0xC8); // 扫描方向, 0xC8从右到左, 0xC0从左到右
    _LCD9648_writeCmd(0xA0); // 段选方向?(决定了内存数据在, 显示器像素点的映射) 0xA0正常方向, 0xA1反方向
    _LCD9648_writeCmd(0x2F); // 0x2F命令, 启动 LCD9648, 不一定显示内容

    _LCD9648_writeCmd(0x26); // 发送0x26 开始对比度控制
    _LCD9648_writeCmd(0x81); // 发送0x81 开启设置对比度值设置
    _LCD9648_writeCmd(0x10); // 设置具体对比度 0x10

    _LCD9648_writeCmd(0xAF); // 发送0xAF 显示屏开启显示内容
}

void LCD9648_clear(void) {
    u8 i = 0;
    u8 j = 0;
    LCD9648_CS0 = 1; // 空闲
    LCD9648_SCL = 0; // 空闲

    // 滚动方向
    _LCD9648_writeCmd(0x40);
    for (i = 0; i < 6; i++) {
        // 页选
        _LCD9648_writeCmd(0xB0 + i); // 0xB0 应该是首页地址

        // 列地址设置
        _LCD9648_writeCmd(0x10); // 列地址, 高位0001
        _LCD9648_writeCmd(0x00); // 列地址, 低位0000

        // 写入数据时, 下一个写入地址会自增
        for (j = 0; j < LCD_WIDTH; j++)
            _LCD9648_writeData(0x00);
    }
}

// font_h: 表示汉字占用的像素宽度大小, 取值16, 24
void LCD9648_showChinese(u8 x, u8 y, u8 font_h, const char* str) {
    u8 u8size = font_h * font_h / 8; // 汉字编码的字节数
    u8 i = 0;                        // 汉字编码遍历索引
    u8 j = 0;                        // 要显示的汉字在CnChar数组中的索引
    u8 x1 = 0;                       // 横索引的低4位
    u8 x2 = 0;                       // 横索引的高4位

    const char* cur = str;
    // 记录font中汉字显示编码数组中元素个数, 用于汉字编码对比
    u8 num = (font_h == 16) ? (sizeof(CnChar16x16) / sizeof(CnChar16x16[0])) : (sizeof(CnChar24x24) / sizeof(CnChar24x24[0]));
    // 记录要从哪个汉字编码数组中找
    const struct CnCharTypeDef* CnChar = (font_h == 16) ? CnChar16x16 : CnChar24x24;

    // 校验参数, x,y,str
    if (x > 95 || y > 5 || !cur)
        return;

    // 遍历字符串, UTF-8编码每3个字节表示一个汉字
    while (*cur != '\0') {
        if ((x > (LCD_WIDTH - font_h)) || (y > (LCD_HEIGHT - 1 - 1)))
            break;

        x1 = x & 0x0F;        //低4位
        x2 = (x >> 4) & 0x0F; //高4位

        // 查询要显示的汉字是不是在font的数组中
        for (j = 0; j < num; j++) {
            // 在font汉字数组中, 找到要显示的汉字
            if ((CnChar[j].Index[0] == *cur) && (CnChar[j].Index[1] == *(cur + 1)) && (CnChar[j].Index[2] == *(cur + 2))) {
                // 遍历汉字编码数组
                y = 0; // 开始写入汉字时, 页偏移量清零, 都从头开始嘛
                for (i = 0; i < u8size; i++) {
                    // 当前字的当前页的写完了, 换下一个页
                    // 不能单纯的判断 i == 点阵宽度, 因为 一个汉字点阵宽度不同, 占用的页就不同
                    if (i % font_h == 0) {
                        _LCD9648_writeCmd(0xB0 | (y++)); // 页地址指令
                        _LCD9648_writeCmd(0x10 | x2);    // x索引高4位指令
                        _LCD9648_writeCmd(0x00 | x1);    // x索引低4位指令
                    }
                    _LCD9648_writeData(CnChar[j].Msk[i]);
                }

                x += font_h; // 每显示一个汉字横索往向移动font_wh个字节
                break;
            }
        }

        cur += 3; // 下一个汉字, 这里自增的大小 要根据不同编码中 汉字字节数来决定, utf-8中 汉字就占用3字节
    }
}

void LCD9648_showImage(u8* imageMap) {
    u8 i = 0;
    u8 j = 0;

    // 滚动方向
    for (i = 0; i < 6; i++) {
        _LCD9648_writeCmd(0x40);
        // 页选
        _LCD9648_writeCmd(0xB0 + i); // 0xB0 应该是首页地址

        // 列地址设置
        _LCD9648_writeCmd(0x10); // 列地址, 高位0001
        _LCD9648_writeCmd(0x00); // 列地址, 低位0000

        // 写入数据时, 下一个写入地址会自增
        for (j = 0; j < LCD_WIDTH; j++) {
            _LCD9648_writeData((*imageMap));
            imageMap++;
        }
    }
}

// 宽度8, 高度16, 编码16字节
void LCD9648_showString(u8 x, u8 y, const char* str) {
    const char* cur = str;
    if (x > LCD_WIDTH || y > LCD_HEIGHT / 8 || !cur)
        return;

    while (*cur != '\0') {
        if (x > 96)
            break;
        _LCD9648_showChar(x, y, *cur);
        x += 8;
        cur++;
    }
}

void LCD9648_show16HZ(u8 x, u8 y, const u8* cnMap);

void LCD9648_showNum(u8 x, u8 y, u32 num, u8 len, u8 mode);

// font.h自带的字符取模数组 是逐列式扫描取模的, 逐列式 会将第一列16个像素 扫出来紧挨着放,
// 字符是8x16的, 而 一页是8, 所以 font.h 里的字符取模, 偶数坐标是第0页的像素取模数据, 奇数坐标是第1页的像素取模数据
// 所以, 写数据时要跳着写
static void _LCD9648_showChar(u8 x, u8 y, char c) {
    u8 u8size = 16; // 一个字符的取模编码长度是16字节
    u8 i = 0;
    u8 x1 = 0;          // 高四位
    u8 x2 = 0;          // 低四位
    u8 index = c - ' '; // 字符编码表的顺序, 是按照ASCII码表 排列的, 第0个取模编码数组是' ', 后续是按照ASCII码表排序的
                        // 所以 字符 - ' ', 起始就是 字符在取模表中的下标位置

    x1 = x & 0x0F;        //低4位
    x2 = (x >> 4) & 0x0F; //高4位

    _LCD9648_writeCmd(0xb0 | y);  //页地址指令
    _LCD9648_writeCmd(0x10 | x2); //x索引高4位指令
    _LCD9648_writeCmd(0x00 | x1); //x索引低4位指令
    for (i = 0; i < u8size / 2; i++) {
        _LCD9648_writeData(ascii_1608[index][i * 2]);
    }

    _LCD9648_writeCmd(0xb0 | (y + 1)); //页地址指令
    _LCD9648_writeCmd(0x10 | x2);      //x索引高4位指令
    _LCD9648_writeCmd(0x00 | x1);      //x索引低4位指令
    for (i = 0; i < u8size / 2; i++) {
        _LCD9648_writeData(ascii_1608[index][i * 2 + 1]);
    }
}

static void _LCD9648_writeData(u8 u8data) {
    u8 i = 0;

    LCD9648_SCL = 0; // 时钟拉低, 空闲
    delayUs(10);

    LCD9648_CS0 = 0; // 片选拉低
    delayUs(10);
    LCD9648_RS = 1; // 写数据
    // MSB 先发高位
    for (i = 0; i < 8; i++) {
        LCD9648_SDA = (u8data & 0x80) ? 1 : 0; // 取高位
        u8data <<= 1;                          // 去高位

        LCD9648_SCL = 1; // 时钟拉高, 发送
        delayUs(10);
        LCD9648_SCL = 0; // 时钟拉低, 空闲
        delayUs(10);
    }

    LCD9648_SCL = 1; // 时钟拉高, 发送
    delayUs(10);
    LCD9648_SCL = 0; // 时钟拉低, 空闲
    delayUs(10);
    LCD9648_CS0 = 1; // 片选拉高
}

static void _LCD9648_writeCmd(u8 u8cmd) {
    u8 i = 0;

    LCD9648_SCL = 0; // 时钟拉低, 空闲
    delayUs(10);
    LCD9648_CS0 = 0; // 片选拉低
    delayUs(10);
    LCD9648_RS = 0; // 写命令

    // MSB 先发高位
    for (i = 0; i < 8; i++) {
        LCD9648_SDA = (u8cmd & 0x80) ? 1 : 0; // 取高位
        u8cmd <<= 1;                          // 去高位

        LCD9648_SCL = 1; // 时钟拉高, 发送
        delayUs(10);
        LCD9648_SCL = 0; // 时钟拉低, 空闲
        delayUs(10);
    }

    LCD9648_SCL = 1; // 时钟拉高, 发送
    delayUs(10);
    LCD9648_SCL = 0; // 时钟拉低, 空闲
    delayUs(10);
    LCD9648_CS0 = 1; // 片选拉高
}

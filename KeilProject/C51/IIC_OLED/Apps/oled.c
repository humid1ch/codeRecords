#include "oled.h"
#include "ssd1306.h"
#include "font.h"

// 写命令
static void _OLED_writeCmd(u8 u8cmd);
// 写数据
static void _OLED_writeData(u8 u8data);
// 通信建立
static void _OLED_writeDataBegin();
// 通信关闭
static void _OLED_writeDataOver();
// 频繁建立 关闭通信的写数据函数
static void _OLED_writeData_ECC(u8 u8data);
static void _OLED_showChar(u8 x, u8 y, char c);

// OLED 初始化
void OLED_init() {
    _OLED_writeCmd(0xAE); // 关闭显示
    _OLED_writeCmd(0xD5); // 设置时钟分频因子,震荡频率
    _OLED_writeCmd(0x80); // [3:0],分频因子;[7:4],震荡频率
    _OLED_writeCmd(0xA8); // 设置驱动路数
    _OLED_writeCmd(0X3F); // 默认0X3F(1/64)
    _OLED_writeCmd(0xD3); // 设置显示偏移
    _OLED_writeCmd(0X00); // 默认为0

    _OLED_writeCmd(0x40); // 设置显示开始行 [5:0],行数.

    _OLED_writeCmd(0x8D); // 电荷泵设置
    _OLED_writeCmd(0x14); // bit2，开启/关闭
    _OLED_writeCmd(0x20); // 设置内存地址模式
    _OLED_writeCmd(0x02); // [1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
    _OLED_writeCmd(0xA1); // 段重定义设置,bit0:0,0->0;1,0->127;
    _OLED_writeCmd(0xC8); // 设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
    _OLED_writeCmd(0xDA); // 设置COM硬件引脚配置
    _OLED_writeCmd(0x12); // [5:4]配置

    _OLED_writeCmd(0x81); // 对比度设置
    _OLED_writeCmd(0xEF); // 1~255;默认0X7F (亮度设置,越大越亮)
    _OLED_writeCmd(0xD9); // 设置预充电周期
    _OLED_writeCmd(0xf1); // [3:0],PHASE 1;[7:4],PHASE 2;
    _OLED_writeCmd(0xDB); // 设置VCOMH 电压倍率
    _OLED_writeCmd(0x30); // [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

    _OLED_writeCmd(0xA4); // 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
    _OLED_writeCmd(0xA6); // 设置显示方式;bit0:1,反相显示;0,正常显示
    _OLED_writeCmd(0xAF); // 开启显示
}

void OLED_clear() {
    u8 i = 0;
    u8 j = 0;

    for (i = 0; i < 8; i++) {
        // 页选
        _OLED_writeCmd(0xB0 + i); // 0xB0 应该是首页地址

        // 列地址设置
        _OLED_writeCmd(0x10); // 列地址, 高位0001
        _OLED_writeCmd(0x00); // 列地址, 低位0000

        //_OLED_writeDataBegin();
        // 写入数据时, 下一个写入地址会自增
        for (j = 0; j < 128; j++)
            _OLED_writeData_ECC(0x00);
        //_OLED_writeDataOver();
    }
}

// 显示图片
void OLED_showImage(const u8* imageMap) {
    u8 i = 0;
    u8 j = 0;

    for (i = 0; i < 8; i++) {
        // 页选
        _OLED_writeCmd(0xB0 + i); // 0xB0 首页地址

        // 列地址设置
        _OLED_writeCmd(0x10); // 列地址, 高位0001
        _OLED_writeCmd(0x00); // 列地址, 低位0000

        //_OLED_writeDataBegin();
        // 写入数据时, 下一个写入地址会自增
        for (j = 0; j < 128; j++) {
            _OLED_writeData_ECC((*imageMap));
            imageMap++;
        }
        //_OLED_writeDataOver();
    }
}
// 显示中文字符串
void OLED_showChinese(u8 x, u8 y, u8 font_w, const char* str) {
    u8 u8size = font_w * font_w / 8; // 汉字编码的字节数
    u8 tempY = y;
    u8 i = 0;                        // 汉字编码遍历索引
    u8 j = 0;                        // 要显示的汉字在CnChar数组中的索引
    u8 x1 = 0;                       // 横索引的低4位
    u8 x2 = 0;                       // 横索引的高4位

    char* cur = str;
    // 记录font中汉字显示编码数组中元素个数, 用于汉字编码对比
    u8 num = (font_w == 16) ? (sizeof(CnChar16x16) / sizeof(CnChar16x16[0])) : (sizeof(CnChar24x24) / sizeof(CnChar24x24[0]));
    // 记录要从哪个汉字编码数组中找
    struct CnCharTypeDef* CnChar = (font_w == 16) ? CnChar16x16 : CnChar24x24;

    // 校验参数, x,y,str
    if (x > 128 || y > 8 || !cur)
        return;

    // 遍历字符串, UTF-8编码每3个字节表示一个汉字
    while (*cur != '\0') {
        if ((x > (OLED_WIDTH - font_w)) || (y > (OLED_HEIGHT - 1 - 1)))
            break;

        x1 = x & 0x0F;        //低4位
        x2 = (x >> 4) & 0x0F; //高4位

        // 查询要显示的汉字是不是在font的数组中
        for (j = 0; j < num; j++) {
            // 在font汉字数组中, 找到要显示的汉字
            if ((CnChar[j].Index[0] == *cur) && (CnChar[j].Index[1] == *(cur + 1)) && (CnChar[j].Index[2] == *(cur + 2))) {
                // 遍历汉字编码数组
                y = tempY; // 开始写入汉字时, 页偏移量清零, 都从头开始嘛
                for (i = 0; i < u8size; i++) {
                    // 当前字的当前页的写完了, 换下一个页
                    // 不能单纯的判断 i == 点阵宽度, 因为 一个汉字点阵宽度不同, 占用的页就不同
                    if (i % font_w == 0) {
                        _OLED_writeCmd(0xB0 | (y++)); // 页地址指令
                        _OLED_writeCmd(0x10 | x2);    // x索引高4位指令
                        _OLED_writeCmd(0x00 | x1);    // x索引低4位指令
                    }
                    _OLED_writeData_ECC(CnChar[j].Msk[i]);
                }

                x += font_w; // 每显示一个汉字横索往向移动font_wh个字节
                break;
            }
        }

        cur += 3; // 下一个汉字, 这里自增的大小 要根据不同编码中 汉字字节数来决定, utf-8中 汉字就占用3字节
    }
}
// 显示英文字符串
void OLED_showString(u8 x, u8 y, const char* str) {
    char* cur = str;

    if (x > OLED_WIDTH || y > OLED_HEIGHT / 8 || !cur)
        return;

    while (*cur != '\0') {
        if (x > OLED_WIDTH)
            break;
        _OLED_showChar(x, y, *cur);
        x += 8;
        cur++;
    }
}

static void _OLED_showChar(u8 x, u8 y, char c) {
    u8 u8size = 16; // 一个字符的取模编码长度是16字节
    u8 i = 0;
    u8 x1 = 0;          // 高四位
    u8 x2 = 0;          // 低四位
    u8 index = c - ' '; // 字符编码表的顺序, 是按照ASCII码表 排列的, 第0个取模编码数组是' ', 后续是按照ASCII码表排序的
                        // 所以 字符 - ' ', 起始就是 字符在取模表中的下标位置

    x1 = x & 0x0F;        //低4位
    x2 = (x >> 4) & 0x0F; //高4位

    _OLED_writeCmd(0xB0 | y);  //页地址指令
    _OLED_writeCmd(0x10 | x2); //x索引高4位指令
    _OLED_writeCmd(0x00 | x1); //x索引低4位指令

    //_OLED_writeDataBegin();
    for (i = 0; i < u8size / 2; i++) {
        _OLED_writeData_ECC(ascii_1608[index][i * 2]);
    }
    //_OLED_writeDataOver();

    _OLED_writeCmd(0xB0 | (y + 1)); //页地址指令
    _OLED_writeCmd(0x10 | x2);      //x索引高4位指令
    _OLED_writeCmd(0x00 | x1);      //x索引低4位指令

    //_OLED_writeDataBegin();
    for (i = 0; i < u8size / 2; i++) {
        _OLED_writeData_ECC(ascii_1608[index][i * 2 + 1]);
    }
    //_OLED_writeDataOver();
}

// 写命令
static void _OLED_writeCmd(u8 u8cmd) {
    SSD1306_writeCmd(u8cmd);
}

// 通信建立
static void _OLED_writeDataBegin() {
    SSD1306_writeDataBegin();
}

// 通信结束
static void _OLED_writeDataOver() {
    SSD1306_writeDataOver();
}

// 不进行通信的建立和关闭, 只负责发数据
static void _OLED_writeData(u8 u8data) {
    SSD1306_writeData(u8data);
}

// 每次发送数据均 重新建立 和 结束通信
static void _OLED_writeData_ECC(u8 u8data) {
    SSD1306_writeData_ECC(u8data);
}

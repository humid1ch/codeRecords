/*
C51 的串口通信 与 定时器有关
需要设置定时器, 来实现串口按一定的波特率发送或接收数据

设置定时器之后, 串口模块会 自动控制 按照波特率发送 或 接收数据
当发送了一帧数据之后, MCU会自动将TI设置为1, 需要手动清零 才能继续发送数据
接收同理, 会将RI自定设置为1, 需要手动清零 才能继续接收数据
       7   6   5   4   3   2   1   0
SCON: SM0 SM1 SM2 REN TB8 RB8 TI  RI

规定上, C51串口通信是全双工的, 但实际上 发送 和 接收数据 公用同一个缓冲区(SBUF 8位寄存器)
所以, 在实际上 C51串口通信只能是半双工的

SBUF 8位寄存器, 是串口通信的一个接收和发送缓冲区, 是需要一次性接收8个bit(1字节)的寄存器, 不能按位寻址
*/

#include <reg52.h>

#define IS_RECEIVE_N(status) (status & 0x8000)
#define RECEIVED_N(status) (status |= 0x8000)

#define IS_RECEIVE_R(status) (status & 0x4000)
#define RECEIVED_R(status) (status |= 0x4000)

unsigned int uartRxStatus = 0;
// 0 ~ 15
// 如果, 接收到'\r', 就将14位 置1
// 如果, 接收到'\n', 就将15位 置1
// 前 0 ~ 13位, 记录接收数据的长度
// 当 14 15 位为 1时, 后面的数据不再接收, 及时进行处理
// 当 14 位为1时, 当前接收的数据必须为'\n', 否则就丢弃并清空已接收数据

char uartBuffer[51] = {0};
char inIndex = 0;

char out = 0; // 是否进行输出

// 规定 发送端 发送数据的结尾为 "\r\n"
void uart1Receive() {
    unsigned char receiveByte = 0;
    receiveByte = SBUF;                        // 接收一下

    if (IS_RECEIVE_N(uartRxStatus) == 0) {     // 没收到 '\n' 再继续接收
        if (IS_RECEIVE_R(uartRxStatus) == 0) { // 还没收到'\r'
            uartBuffer[inIndex++] = receiveByte;
            
        }
        else {
            if (receiveByte == '\n')
                RECEIVED_N(uartRxStatus);
            else {
            }
        }
    }
}

void uart1Func(void) interrupt 4 {
    //检测串口1接收中断
    if (RI) {

        uartBuffer[inIndex++] = SBUF;

        RI = 0; //清除串口1接收中断请求位

        // 判断 数据是否接收完毕
        out = receiveOver(uartBuffer, inIndex);
    }
}

// uart1 串口1 用的是定时器1
void uart1Init(void) {
    PCON |= 0x80; //使能波特率倍速位SMOD
    SCON = 0x50;  //8位数据,可变波特率

    // 先高4位清零, 在设置计时器模式
    TMOD &= 0x0F; // 定时器1 高4位清零
    TMOD |= 0x20; // 定时器1 为定时器模式, 8位 自动重载

    // 8位 自动重载
    TL1 = 0xFA; // 设置定时初始值
    TH1 = 0xFA; // 设置定时重载值

    ET1 = 0; // 禁止定时器中断, 因为要串口1中断
    TR1 = 1; // 定时器1开始计时

    ES = 1; // 使能串口1中断
    EA = 1;
}

void main() {
    char i = 0;

    uart1Init();

    while (1) {
        if (out) {
            for (i = 0; i <= inIndex; i++) {
                SBUF = uartBuffer[i];
                while (!TI)
                    ;
                TI = 0;

                uartBuffer[i] = 0; // 清除缓冲区内容

                if (i == inIndex) {
                    inIndex = 0; // 清空缓冲区
                }
            }

            out = 0;
        }
    }
}
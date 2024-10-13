#include "reg52.h"
#include "infrared.h"

sbit IRED = P3 ^ 2; // 红外接收器电信号输出引脚

u8 gIred_code[4] = {0}; //NEC码（地址码，地址反码，控制码，控制反码）

void IRD_Init(void) {
    IT0 = 1; //设置外部中断0为下降沿触发
    EX0 = 1; // 外部中断0使能
    EA = 1;  //总中断使能

    IRED = 1; //红外端口空闲为高电平
}

// 红外解NEC码, 接收器收到红外信号会触发外部中断0
// 中断函数, 不用暴露
void Ired_Int0(void) interrupt 0 {
    u8 i = 0, j = 0;
    u16 time_out = 1000;

    if (IRED == 0) {
        // 1. 分析引导码
        // 等待引导信号低电平结束, 若超时 即超过10ms, 则退出
        // 因为 引导信号的低电平应该是9ms左右, 可以少, 但是不能多太多
        while (!IRED && time_out) {
            time_out--;
            delay10Us();

            if (time_out == 0)
                return; // 10ms超时
        }
        // 出循环, 说明 低电平信号持续时间没有超时
        // 引脚电平已经拉高
        if (IRED) {
            time_out = 500;
            // 等待引导信号高电平结束, 若超时 则退出
            // 高电平要持续4.5ms左右, 这里等待5ms
            while (IRED && time_out) {
                time_out--;
                delay10Us();
                if (time_out == 0)
                    return; // 5ms超时
            }
        }

        // 2. 分析32位数据 (8位地址码原反 和 8位控制码原反)
        // 4个8位码
        for (i = 0; i < 4; i++) {
            gIred_code[i] = 0;
            // 地址码 和 控制码都是8位
            for (j = 0; j < 8; j++) {
                // 0 和 1, 都是先有 0.56ms 的低电平
                time_out = 60; // 等待0.6ms低电平结束，若超时则退出
                while (!IRED && time_out) {
                    time_out--;
                    delay10Us();
                    if (time_out == 0)
                        return; // 0.6ms超时
                }
                // 高电平时间不会超过2000us,
                time_out = 20; // 等待2ms高电平结束, 若超时则退出
                while (IRED) {
                    time_out--;
                    delayN10Us(10); // 由于C51速度不够，只能100us
                    if (time_out == 0)
                        return; // 2ms超时
                }

                // 读到是1, 再记录, 读到是0, 可以不做处理
                // 数据是 LSB 发送过来的, 所以先收到的是低位
                if (time_out < 8) {
                    // time_out < 8, 就表示收到的是 1
                    // 因为 如果是0, 高电平的持续时间不会太久, 即等待的过程中的time_out不会--到小于8
                    gIred_code[i] |= 1 << j;
                }
            }
        }

        // 3. 校检数据
        // 如果 接收到的 控制码的原码 和 反码 不能匹配
        // 说明 数据错误, 置零, 可以让后续不做处理
        if (gIred_code[2] != ~gIred_code[3]) {
            gIred_code[2] = 0;
            gIred_code[3] = 0;
        }
    }
}
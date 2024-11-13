#include "stm32f10x_it.h"
#include "utils.h"
#include "stdio.h"
#include "string.h"
#include "timer.h"
#include "ds18b20.h"
#include "smg.h"
#include "usart.h"
#include "esp8266.h"
#include "beep.h"
#include "matrix_key.h"
#include "oled.h"
#include "ssd1306.h"
#include "rtc.h"

#define USART_REC_LEN 128 // 最大串口缓冲区

char USART3_RX_BUF[USART_REC_LEN] = {0}; // 串口3的接收缓冲
u16 USART3_RX_STA = 0;                   // 串口3接收状态标记+接收个数

char USART1_RX_BUF[USART_REC_LEN] = {0}; // 串口1的接收缓冲
u16 USART1_RX_STA = 0;                   // 串口1接收状态标记+接收个数

#define WIFI_SSID "ESP_humid1ch"
#define WIFI_PASSWD "dxyt200209."
#define WIFI_CHANNEL 13 // 信道: 0~13
#define WIFI_PSSWD_MODE 4
#define ESP_LOCAL_IP "192.168.9.148"
#define ESP_SERVER_PORT 8088

void ESP8266_serverConfig(void);

typedef struct DS18B20_Data {
    u16 u16data;         // 读取整型数据
    u8 negFlag;          // 正负号
    float temperature;   // 无符号 最终温度
    float thresholdTemp; // 阈值温度, 用于警报
} DS18B20_Data;

// 温度计算函数
void DS18B20_tempCalculate(DS18B20_Data* tempData);
// OLED首次显示函数, 在第一次显示时调用
void OLED_startDisplay(const DS18B20_Data* tempData);
// OLED温度更新 显示函数
void OLED_updateTempDisplay(const DS18B20_Data* tempData);
// OLED日期时间更新 显示函数
void OLED_updateDateDisplay(void);
// OLED阈值更新 显示函数, 在阈值变动时执行
void OLED_updateThresholdDisplay(const DS18B20_Data* tempData);
// 数码管显示温度 函数
void SMG_tempDisplay(const DS18B20_Data* tempData);
// 通过网络发送温度数据
void ESP8266_tempSend(const DS18B20_Data* tempData);
// 接收网络命令, 并进行处理
void ESP8266_receiveResponse(DS18B20_Data* tempData);
// 阈值警报
void BEEP_alertsThreshold(const DS18B20_Data* tempData);

u8 tempUpdate = 1; // 记录 温度是否需要更新显示
u8 sendFlag = 0;   // 数据发送标记位, 用于记录是否需要进行数据发送
u8 dateUpdate = 1; // 记录 时间是否需要更新显示
u8 oledFirst = 1;  // 记录 oled是否为初次显示
u8 threUpdate = 0; // 记录 阈值是否需要更新显示
u8 beepOn = 1;     // 用于手动开关蜂鸣器

int main() {
    SysTick_init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // RTC 时钟初始化
    RTC_init();
    // 串口1 115200 波特率初始化
    USART1_init(115200); // 调试用, 用于SSCOM串口1显示
    // 串口3 115200 波特率初始化
    USART3_init(115200); // ESP8266 模块输入输出用

    // ESP8266 Wifi模块 初始化
    ESP8266_init();
    delayMs(500);
    // ESP8266 服务配置(作为服务端)
    ESP8266_serverConfig();
    delayMs(500);

    // DS18B20 温度传感器初始化
    DS18B20_init();

    // 数码管 初始化
    SMG_init();
    // OLED的驱动 SSD1306 初始化(硬IIC)
    SSD1306_initHard();
    // OLED 初始化
    OLED_init();
    // 蜂鸣器初始化
    BEEP_init();
    // 矩阵按键初始化
    MKEY_init();
    // 定时器初始化
    TIMER_init(TIM2, 4000, 36000 - 1); // 定时器的频率是2KHz, 周期为3000ms
    TIMER_init(TIM3, 2000, 36000 - 1); // 定时器的频率是2KHz, 周期为1000ms

    // 温度数据结构体
    DS18B20_Data temperatureData;
    // 默认温度阈值
    temperatureData.thresholdTemp = 32.0;

    while (1) {
        // 是否需要计算并更新温度
        if (tempUpdate) {
            DS18B20_tempCalculate(&temperatureData);
            // 更新完成清除标记
            tempUpdate = 0;
            // 更新完成, 需要通过Wifi发送数据
            sendFlag = 1;

            if (oledFirst) {
                OLED_startDisplay(&temperatureData);
                // 首次显示完毕
                oledFirst = 0;
            }
            else
                OLED_updateTempDisplay(&temperatureData);
        }

        // 是否需要更新 时间
        if (dateUpdate) {
            OLED_updateDateDisplay();
            // 更新完成, 清除标记
            dateUpdate = 0;
        }

        // 阈值是否需要更新
        if (threUpdate) {
            OLED_updateThresholdDisplay(&temperatureData); // 阈值变动时, 在OLED更新阈值
            // 更新完成, 清楚标记
            threUpdate = 0;
        }

        // 是否需要通过 Wifi 向客户端发送数据
        // sendFlag 在进行完温度计算之后, 被置1, 表示进行数据发送
        if (sendFlag) {
            ESP8266_tempSend(&temperatureData); // 通过Wifi发送数据

            // 发送完毕
            sendFlag = 0;
        }

        // 如果ESP8266 收到一个完整的字符串, 就对接收的数据进行处理
        if (USART3_RX_STA & 0x8000)
            ESP8266_receiveResponse(&temperatureData); // 处理接收到的字符串

        // 判断 蜂鸣器是否允许被开启
        if (beepOn)
            BEEP_alertsThreshold(&temperatureData); // 判断是否需要 开启警报

        // 数码管显示温度函数
        SMG_tempDisplay(&temperatureData);

        // 矩阵按键扫描: MK1: 阈值+1, MK2: 阈值-1, MK3: 阈值+0.1, MK4: 阈值-0.1, MK5: 开启或关闭蜂鸣器开关
        u16 keyRes = MKEY_keyScan();
        switch (keyRes) {
        case MKEY1_PRESSED:
            temperatureData.thresholdTemp += 1;
            OLED_updateThresholdDisplay(&temperatureData); // 阈值变动时, 在OLED更新阈值
            break;

        case MKEY2_PRESSED:
            temperatureData.thresholdTemp -= 1;
            OLED_updateThresholdDisplay(&temperatureData);
            break;

        case MKEY3_PRESSED:
            temperatureData.thresholdTemp += 0.1;
            OLED_updateThresholdDisplay(&temperatureData);
            break;

        case MKEY4_PRESSED:
            temperatureData.thresholdTemp -= 0.1;
            OLED_updateThresholdDisplay(&temperatureData);
            break;

        case MKEY5_PRESSED:
            beepOn = !beepOn; // 控制蜂鸣器总开关
            break;

        default:
            break;
        }
    }
}

// 蜂鸣器报警, 每次给一个脉冲, 满足条件会循环脉冲, 可以避免长时间delay影响数码管显示和WIFI接收数据
void BEEP_alertsThreshold(const DS18B20_Data* tempData) {
    if (tempData->temperature - tempData->thresholdTemp > 0.0001) {
        if (tempData->temperature - tempData->thresholdTemp > 0.0001) {
            BEEP_pulse50Us(); // 给一个 50Us的脉冲, 不要太长, 不然影响显示和收数据
        }
    }
}

void ESP8266_tempSend(const DS18B20_Data* tempData) {
    // 通过WiFi发送 "temp: xx.x, threshold: xx.x" 字符串

    char cmd[64] = {0};
    sprintf(cmd, "temp: %.1f, threshold: %.1f", tempData->temperature, tempData->thresholdTemp);
    sprintf(cmd, "AT+CIPSEND=0,%u\r\n", strlen(cmd)); // 发送 strlen(cmd)长度的数据
    ESP8266_sendATCmd(cmd);
    delayMs(2);

    // 发送数据
    sprintf(cmd, "temp: %.1f, threshold: %.1f", tempData->temperature, tempData->thresholdTemp);
    ESP8266_sendATCmd(cmd);
    delayMs(2);
}

// ESP8266 接收到的数据会存储在定义的USART3的接收缓冲区中
void ESP8266_receiveResponse(DS18B20_Data* tempData) {
    // 判断是否完整收到了一个包含字符串 \r\n的字符串
    // 判断命令: MODTHRE(修改报警温度阈值)  BEEPON(打开蜂鸣器指令)  BEEPOFF(关闭蜂鸣器指令)
    // 客户端若要修改报警温度阈值, 发送的数据格式必须为 "MODTHRE: xx(整数部分) x(小数部分)"
    // 因为温度格式为 2位整数+1位小数, 方便接收且分明, 所以将 整数部分和小数部分 以整数的形式分开发送, 并接收 处理
    // 所以, 修改温度报警阈值的指令为: MODTHRE: xx x
    char* cur = strstr(USART3_RX_BUF, "MODTHRE: ");

    // 分析命令
    if (cur != NULL) {
        int thresholdTempTen = 0;
        int thresholdTempUnit = 0;
        char appCmd[16] = {0};
        sscanf(USART3_RX_BUF, "%s %d %d", appCmd, &thresholdTempTen, &thresholdTempUnit);
        // 更新阈值
        tempData->thresholdTemp = thresholdTempTen + (float)thresholdTempUnit / 10;
        // 更新阈值显示
        OLED_updateThresholdDisplay(tempData);
    }

    // 关闭蜂鸣器
    cur = strstr(USART3_RX_BUF, "BEEPOFF");
    // 分析命令
    if (cur != NULL) {
        beepOn = 0;
    }

    // 打开蜂鸣器
    cur = strstr(USART3_RX_BUF, "BEEPON");
    // 分析命令
    if (cur != NULL) {
        beepOn = 1;
    }

    // 清空缓冲区
    // 数据清零
    int dataLen = USART3_RX_STA & 0x3FFF;
    for (u16 i = 0; i < dataLen; i++)
        USART3_RX_BUF[i] = 0;

    USART3_RX_STA = 0;
}

void DS18B20_tempCalculate(DS18B20_Data* tempData) {
    tempData->u16data = DS18B20_readTemperature();

    // 负数修正, 但一般不会出现
    if (tempData->u16data & 0xF800) {
        tempData->u16data = (~tempData->u16data) + 1;
        tempData->negFlag = 1;
    }

    tempData->temperature = tempData->u16data * 0.0625;
}

void SMG_tempDisplay(const DS18B20_Data* tempData) {
    u8 smgCode[8] = {0};

    // 一般不会出现负数, 所以暂时不对负数做处理
    u16 tempInt = 0;
    u16 thresholdTempInt = 0;
    tempInt = (u16)(tempData->temperature * 10);
    thresholdTempInt = (u16)(tempData->thresholdTemp * 10);
    smgCode[0] = tempInt / 100 % 10;
    smgCode[1] = tempInt / 10 % 10;
    smgCode[2] = tempInt % 10;

    smgCode[4] = thresholdTempInt / 100 % 10;
    smgCode[5] = thresholdTempInt / 10 % 10;
    smgCode[6] = thresholdTempInt % 10;

    // 摄氏度符号显示
    // C 是 12
    smgCode[3] = 12;
    smgCode[7] = 12;

    // 数码管显示
    SMG_display(smgCode);
    // 小数点显示
    smgFuncs[1]();
    SMG_writeByte(SMG_SEG_PORT, smgNum[16], WRITE_L8);
    delayUs(2);
    SMG_writeByte(SMG_SEG_PORT, 0x00, WRITE_L8);

    smgFuncs[5]();
    SMG_writeByte(SMG_SEG_PORT, smgNum[16], WRITE_L8);
    delayUs(2);
    SMG_writeByte(SMG_SEG_PORT, 0x00, WRITE_L8);
}

void OLED_startDisplay(const DS18B20_Data* tempData) {
    // 年-月-日 xxxx-xx-xx
    char dateStr[12] = {0};
    sprintf(dateStr, "%d-%02d-%02d", nowDate.year, nowDate.month, nowDate.day);

    // 时:分:秒 xx:xx:xx
    char timeStr[10] = {0};
    sprintf(timeStr, "%02d:%02d:%02d", nowDate.hour, nowDate.minute, nowDate.second);

    // temp: xx.xC
    char tempStr[12] = {0};
    sprintf(tempStr, "temp: %.1fC", tempData->temperature);

    // threshold: xx.xC
    char thresholdStr[17] = {0};
    sprintf(thresholdStr, "threshold: %.1fC", tempData->thresholdTemp);

    OLED_clear();
    OLED_showChinese(0, 0, 16, "徐怡潭");
    OLED_showString(63, 0, timeStr);
    OLED_showString(0, 2, dateStr);
    OLED_showString(0, 4, tempStr);
    OLED_showString(0, 6, thresholdStr);
}

void OLED_updateDateDisplay(void) {
    // 年-月-日 xxxx-xx-xx
    // char dateStr[12] = {0};
    // sprintf(dateStr, "%d-%02d-%02d", nowDate.year, nowDate.month, nowDate.day);

    // 时:分:秒 xx:xx:xx
    char timeStr[10] = {0};
    sprintf(timeStr, "%02d:%02d:%02d", nowDate.hour, nowDate.minute, nowDate.second);

    OLED_showString(63, 0, timeStr);
    // OLED_showString(0, 2, dateStr);
}

void OLED_updateTempDisplay(const DS18B20_Data* tempData) {
    // xx.x
    char tempStr[5] = {0};
    sprintf(tempStr, "%.1f", tempData->temperature);

    OLED_showString(48, 4, tempStr);
}

void OLED_updateThresholdDisplay(const DS18B20_Data* tempData) {
    // 阈值更新不该在这, 应该在接收到修改指令时, 更新
    // xx.x
    char thresholdStr[5] = {0};
    sprintf(thresholdStr, "%.1f", tempData->thresholdTemp);

    OLED_showString(88, 6, thresholdStr);
}

void ESP8266_serverConfig(void) {
    char cmdStr[128] = {0}; // 用于存储格式化命令

    sprintf(cmdStr, "AT+CWMODE=2\r\n");
    ESP8266_sendATCmd(cmdStr);
    delayMs(1000);

    sprintf(cmdStr, "AT+CWSAP_DEF=\"%s\",\"%s\",%d,%d\r\n", WIFI_SSID, WIFI_PASSWD, WIFI_CHANNEL, WIFI_PSSWD_MODE);
    ESP8266_sendATCmd(cmdStr);
    delayMs(1000);

    sprintf(cmdStr, "AT+CIPAP_DEF=\"%s\"\r\n", ESP_LOCAL_IP);
    ESP8266_sendATCmd(cmdStr);
    delayMs(1000);

    sprintf(cmdStr, "AT+CIPMUX=1\r\n");
    ESP8266_sendATCmd(cmdStr);
    delayMs(1000);

    sprintf(cmdStr, "AT+CIPSERVER=1,%d\r\n", ESP_SERVER_PORT);
    ESP8266_sendATCmd(cmdStr);
    delayMs(1000);
}

void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
        u16 rec = USART_ReceiveData(USART1); // 从USART1模块的数据寄存器中读取接收到的数据

        // 判断接收是否完成, 即是否已经标记了 收到 '\n'
        if ((USART1_RX_STA & 0x8000) == 0) {
            // 判断有没有收到过 '\r'
            if (USART1_RX_STA & 0x4000) {
                // 数据接收不正常
                if (rec != '\n')
                    USART1_RX_STA = 0;
                else                         // 数据正常, 即收到 \n
                    USART1_RX_STA |= 0x8000; // 标记数据接收完成
            }
            else {
                // 表示接收到的是 \r
                if (rec == '\r')
                    USART1_RX_STA |= 0x4000; // 标记数据接收到\r
                else {
                    // 无论是\r 还是 \n, 都进行存储
                    USART1_RX_BUF[USART1_RX_STA & 0x3FFF] = rec;
                    USART1_RX_STA++;
                    if (USART1_RX_STA > USART_REC_LEN - 1)
                        USART1_RX_STA = 0;
                }
            }
        }
    }
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

// 接收数据的中断
void USART3_IRQHandler(void) {
    if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET) {
        u16 rec = USART_ReceiveData(USART3); // 从USART3模块的数据寄存器中读取接收到的数据

        // 判断接收是否完成, 即是否已经标记了 收到 '\n'
        if ((USART3_RX_STA & 0x8000) == 0) {
            // 判断有没有收到过 '\r'
            if (USART3_RX_STA & 0x4000) {
                // 数据接收不正常
                if (rec != '\n')
                    USART3_RX_STA = 0;
                else                         // 数据正常, 即收到 \n
                    USART3_RX_STA |= 0x8000; // 标记数据接收完成
            }
            else {
                // 表示接收到的是 \r
                if (rec == '\r')
                    USART3_RX_STA |= 0x4000; // 标记数据接收到\r
                else {
                    // 存储 有效数据
                    USART3_RX_BUF[USART3_RX_STA & 0x3FFF] = rec;
                    USART3_RX_STA++;
                    if (USART3_RX_STA > USART_REC_LEN - 1)
                        USART3_RX_STA = 0;
                }
            }
        }
    }
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}

void TIM2_IRQHandler(void) {
    // 触发中断时, 表示温度需要更新
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
        tempUpdate = 1;

    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
void TIM3_IRQHandler(void) {
    // 触发中断时, 表示时间需要更新
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
        dateUpdate = 1;

    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

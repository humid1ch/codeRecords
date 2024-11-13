#include "stm32f10x_it.h"
#include "stdio.h"
#include "utils.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timer.h"
#include "ds18b20.h"
#include "smg.h"

typedef struct DS18B20_Data {
    u16 u16data;         // 读取整型数据
    u8 negFlag;          // 正负号
    float temperature;   // 无符号 最终温度
    float thresholdTemp; // 阈值温度, 用于警报
} DS18B20_Data;

u8 updateFlag = 1; // 更新标记位
u8 sendFlag = 0;   // 数据发送标记位

#define TEMP_CAL_TASK_STK_DEPTH 50
#define TEMP_CAL_TASK_PRIO 3
TaskHandle_t TEMP_CAL_TASK_Handler; // 温度计算任务 的权柄
void DS18B20_tempCalculateTask(void* const pvParameters);

#define TEMP_DIS_TASK_STK_DEPTH 50
#define TEMP_DIS_TASK_PRIO 2
TaskHandle_t TEMP_DIS_TASK_Handler; // 温度显示任务 的权柄
void SMG_tempDisplayTask(void* const pvParameters);

int main() {
    SysTick_init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    TIMER_init(TIM2, 10000, 36000 - 1); // 定时器的频率是2KHz, 周期为2000ms

    DS18B20_init();
    SMG_init();

    static DS18B20_Data temperatureData;
    // 默认温度阈值
    temperatureData.thresholdTemp = 30.0;

    xTaskCreate((TaskFunction_t)DS18B20_tempCalculateTask, (const char*)"tempCalculate", (uint16_t)TEMP_CAL_TASK_STK_DEPTH, (void*)&temperatureData, (UBaseType_t)TEMP_CAL_TASK_PRIO,
                (TaskHandle_t*)&TEMP_CAL_TASK_Handler);
    xTaskCreate((TaskFunction_t)SMG_tempDisplayTask, (const char*)"tempDisplay", (uint16_t)TEMP_DIS_TASK_STK_DEPTH, (void*)&temperatureData, (UBaseType_t)TEMP_DIS_TASK_PRIO,
                (TaskHandle_t*)&TEMP_DIS_TASK_Handler);

    vTaskStartScheduler();
}

void DS18B20_tempCalculateTask(void* const pvParameters) {
    DS18B20_Data* tempData = (DS18B20_Data*)pvParameters;
    while (1) {
        // 需要更新计算温度数据
        if (updateFlag) {
            tempData->u16data = DS18B20_readTemperature();

            // 负数修正, 但一般不会出现
            if (tempData->u16data & 0xF800) {
                tempData->u16data = (~tempData->u16data) + 1;
                tempData->negFlag = 1;
            }

            tempData->temperature = tempData->u16data * 0.0625;

            // 更新完成, 清除标记
            updateFlag = 0;
            // 更新完成, 需要通过Wifi发送数据
            sendFlag = 1;

            vTaskDelay(pdMS_TO_TICKS(200));
        }
    }
}

void SMG_tempDisplayTask(void* const pvParameters) {
    DS18B20_Data* tempData = (DS18B20_Data*)pvParameters;
    u8 smgCode[8] = {0};
    // 摄氏度符号显示
    // C 是 12
    smgCode[3] = 12;
    smgCode[7] = 12;
    // 一般不会出现负数, 所以暂时不对负数做处理
    u16 tempInt = 0;
    u16 thresholdTempInt = 0;
    while (1) {
        tempInt = (u16)(tempData->temperature * 10);
        thresholdTempInt = (u16)(tempData->thresholdTemp * 10);
        smgCode[0] = tempInt / 100 % 10;
        smgCode[1] = tempInt / 10 % 10;
        smgCode[2] = tempInt % 10;

        smgCode[4] = thresholdTempInt / 100 % 10;
        smgCode[5] = thresholdTempInt / 10 % 10;
        smgCode[6] = thresholdTempInt % 10;

        // 数码管显示
        SMG_display(smgCode);
        // 小数点显示
        smgFuncs[1]();
        SMG_writeByte(SMG_SEG_PORT, smgNum[16], WRITE_L8);
        delayMs(2);
        SMG_writeByte(SMG_SEG_PORT, 0x00, WRITE_L8);

        smgFuncs[5]();
        SMG_writeByte(SMG_SEG_PORT, smgNum[16], WRITE_L8);
        delayMs(2);
        SMG_writeByte(SMG_SEG_PORT, 0x00, WRITE_L8);

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void TIM2_IRQHandler(void) {
    // 触发中断时, 更新温度
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        updateFlag = 1;
    }

    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

#include "stm32f10x_it.h"
#include "stdio.h"
#include "utils.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"

int main() {
    SysTick_init(72);

    // xTaskCreate(); 创建任务函数
    // 参数:
    //  1. TaskFunction_t pxTaskCode: 任务 的函数指针
    //  2. const char* const pcName: 任务名, 起个名字
    //  3. const uint16_t usStackDepth: 任务函数 堆栈调用深度
    //  4. void* const pvParamenters: 任务函数参数
    //  5. UBaseType_t uxPriority: 任务优先级
    //  6. TaskHandle_t* const pxCreatedTask: 输出型参数, 用于接收任务创建完成之后的 句柄
    // xTaskCreate((TaskFunction_t), (const char*)"", (uint16_t), (void*)NULL, (UBaseType_t), (TaskHandle_t*));

    vTaskStartScheduler();
}


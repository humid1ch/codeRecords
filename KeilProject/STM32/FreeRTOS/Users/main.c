#include "stm32f10x_it.h"
#include "stdio.h"
#include "utils.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"

#define LED1_TASK_STK_DEPTH 50 // 任务 堆栈深度 单位字节
#define LED1_TASK_PRIO 2       // 优先级

#define LED2_TASK_STK_DEPTH 50 // 任务 堆栈深度 单位字节
#define LED2_TASK_PRIO 0       // 优先级

#define LED3_TASK_STK_DEPTH 50 // 任务 堆栈深度 单位字节
#define LED3_TASK_PRIO 4       // 优先级

TaskHandle_t LED1_TASK_Handler; // LED1_TASK的权柄
TaskHandle_t LED2_TASK_Handler; // LED2_TASK的权柄
TaskHandle_t LED3_TASK_Handler; // LED3_TASK的权柄

void LED1_blinkTask(void* const pvParameters);
void LED2_blinkTask(void* const pvParameters);
void LED3_blinkTask(void* const pvParameters);

int main() {
    SysTick_init(72);
    LED_init();

    static u16 second = 2;

    // xTaskCreate(); 创建任务函数
    // 参数:
    //  1. TaskFunction_t pxTaskCode: 任务 的函数指针
    //  2. const char* const pcName: 任务名, 起个名字
    //  3. const uint16_t usStackDepth: 任务函数 堆栈调用深度
    //  4. void* const pvParamenters: 任务函数参数
    //  5. UBaseType_t uxPriority: 任务优先级
    //  6. TaskHandle_t* const pxCreatedTask: 输出型参数, 用于接收任务创建完成之后的 句柄
    xTaskCreate((TaskFunction_t)LED1_blinkTask, (const char*)"LED1_blinkTask", (uint16_t)LED1_TASK_STK_DEPTH, (void*)NULL, (UBaseType_t)LED1_TASK_PRIO, (TaskHandle_t*)&LED1_TASK_Handler);
    xTaskCreate((TaskFunction_t)LED2_blinkTask, (const char*)"LED2_blinkTask", (uint16_t)LED2_TASK_STK_DEPTH, (void*)NULL, (UBaseType_t)LED2_TASK_PRIO, (TaskHandle_t*)&LED2_TASK_Handler);
    xTaskCreate((TaskFunction_t)LED3_blinkTask, (const char*)"LED3_blinkTask", (uint16_t)LED3_TASK_STK_DEPTH, (void*)&second, (UBaseType_t)LED3_TASK_PRIO, (TaskHandle_t*)&LED3_TASK_Handler);

    vTaskStartScheduler();
}

void LED1_blinkTask(void* const pvParameters) {
    while (1) {
        LED1 = !LED1;
        vTaskDelay(500);
    }
}
void LED2_blinkTask(void* const pvParameters) {
    while (1) {
        LED2 = !LED2;
        vTaskDelay(1000);
    }
}
void LED3_blinkTask(void* const pvParameters) {
    while (1) {
        LED3 = !LED3;
        vTaskDelay(1000 * (*(u16*)pvParameters));
    }
}

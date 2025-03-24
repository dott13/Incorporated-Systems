#include "freertos_tasks.h"
#include <stdio.h>

// Create global semaphore, queue, and mutex.
SemaphoreHandle_t xButtonSemaphore = NULL;
QueueHandle_t xDataQueue = NULL;
SemaphoreHandle_t xMutex = NULL;

volatile int extraCounter = 0; // Shared counter for Task Extra

//--------------------------------------------------
// Task 1: Button LED Task
void vTaskButtonLED(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    bool buttonPrevState = HIGH;
    bool ledOnActive = false;
    TickType_t ledOnTime = 0;

    for (;;)
    {
        bool currentState = digitalRead(PIN_BUTTON);
        if ((buttonPrevState == HIGH) && (currentState == LOW))
        {
            digitalWrite(LED1_PIN, HIGH);
            ledOnActive = true;
            ledOnTime = xTaskGetTickCount();
            xSemaphoreGive(xButtonSemaphore);
        }
        if (ledOnActive)
        {
            if ((xTaskGetTickCount() - ledOnTime) >= pdMS_TO_TICKS(1000))
            {
                digitalWrite(LED1_PIN, LOW);
                ledOnActive = false;
            }
        }
        buttonPrevState = currentState;
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10));
    }
}

//--------------------------------------------------
// Task 2: Synchronization Task
void vTaskSync(void *pvParameters)
{
    static uint8_t N = 0;
    uint8_t value;
    for (;;)
    {
        if (xSemaphoreTake(xButtonSemaphore, portMAX_DELAY) == pdPASS)
        {
            N++;
            printf("Task2: Received semaphore. New N = %d\n", N);
            for (uint8_t i = 1; i <= N; i++)
            {
                value = i;
                if (xQueueSendToFront(xDataQueue, &value, 0) != pdPASS)
                {
                    printf("Task2: Failed to send byte %d to queue\n", i);
                }
                vTaskDelay(pdMS_TO_TICKS(50));
            }
            for (uint8_t j = 0; j < N; j++)
            {
                digitalWrite(LED2_PIN, HIGH);
                vTaskDelay(pdMS_TO_TICKS(300));
                digitalWrite(LED2_PIN, LOW);
                vTaskDelay(pdMS_TO_TICKS(500));
            }
        }
    }
}

//--------------------------------------------------
// Task 3: Asynchronous Task
void vTaskAsync(void *pvParameters)
{
    uint8_t receivedValue;
    char buffer[64];
    for (;;)
    {
        buffer[0] = '\0';
        while (xQueueReceive(xDataQueue, &receivedValue, 0) == pdPASS)
        {
            char temp[4];
            sprintf(temp, "%d ", receivedValue);
            strcat(buffer, temp);
        }
        if (strlen(buffer) > 0)
        {
            printf("Task3: Received from queue: %s\n", buffer);
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

//--------------------------------------------------
// Task Extra: Additional Functionality using Mutex.
void vTaskExtra(void *pvParameters)
{
    for (;;)
    {
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
        {
            extraCounter++;
            printf("TaskExtra: extraCounter = %d\n", extraCounter);
            xSemaphoreGive(xMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

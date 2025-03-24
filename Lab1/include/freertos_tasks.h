#ifndef FREERTOS_TASKS_H
#define FREERTOS_TASKS_H

#include <Arduino.h>

// Use the AVR port of FreeRTOS headers.
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

#define PIN_BUTTON 2
#define LED1_PIN LED_BUILTIN // Task 1 LED
#define LED2_PIN 12          // Task 2 LED

// Task function prototypes.
void vTaskButtonLED(void *pvParameters);
void vTaskSync(void *pvParameters);
void vTaskAsync(void *pvParameters);
void vTaskExtra(void *pvParameters);

// Global FreeRTOS handles.
extern SemaphoreHandle_t xButtonSemaphore;
extern QueueHandle_t xDataQueue;
extern SemaphoreHandle_t xMutex;

#endif

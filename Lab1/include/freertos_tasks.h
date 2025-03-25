#ifndef FREERTOS_TASKS_H
#define FREERTOS_TASKS_H

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

// Pin Definitions
#define PIN_BUTTON 2         // Button input pin
#define LED1_PIN LED_BUILTIN // Built-in LED
#define LED2_PIN 13          // External LED

// Task Function Prototypes
void vTaskButtonLED(void *pvParameters);
void vTaskSync(void *pvParameters);
void vTaskAsync(void *pvParameters);
void vTaskExtra(void *pvParameters);

// Global FreeRTOS Synchronization Objects
extern SemaphoreHandle_t xButtonSemaphore;
extern QueueHandle_t xDataQueue;
extern SemaphoreHandle_t xMutex;

#endif // FREERTOS_TASKS_H
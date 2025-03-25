#include <Arduino.h>
#include <stdio.h>
#include "freertos_tasks.h"

// STDIO redirection functions
static int serial_putchar(char c, FILE *stream)
{
  if (c == '\n')
  {
    Serial.write('\r');
  }
  Serial.write(c);
  return 0;
}

static int serial_getchar(FILE *stream)
{
  while (!Serial.available())
  {
    vTaskDelay(1); // Use FreeRTOS delay instead of busy waiting
  }
  return Serial.read();
}

static FILE serial_stream;

void setup()
{
  // Initialize Serial with multiple attempts
  for (int i = 0; i < 5; i++)
  {
    Serial.begin(115200);
    delay(500);
    if (Serial)
      break;
  }

  // Wait for Serial to be ready with timeout
  unsigned long startTime = millis();
  while (!Serial && (millis() - startTime < 10000))
  {
    // Wait for Serial to be ready
  }

  if (!Serial)
  {
    // Fallback error signaling
    pinMode(LED_BUILTIN, OUTPUT);
    while (1)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
  }

  // Redirect STDIO
  serial_stream.flags = _FDEV_SETUP_WRITE | _FDEV_SETUP_READ;
  serial_stream.put = serial_putchar;
  serial_stream.get = serial_getchar;
  stdout = &serial_stream;
  stdin = &serial_stream;

  printf("System initializing with FreeRTOS on ATMega2560...\n");

  // Configure hardware pins
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // Create FreeRTOS synchronization objects
  xButtonSemaphore = xSemaphoreCreateBinary();
  if (xButtonSemaphore == NULL)
  {
    printf("Failed to create button semaphore!\n");
  }

  xDataQueue = xQueueCreate(20, sizeof(uint8_t));
  if (xDataQueue == NULL)
  {
    printf("Failed to create data queue!\n");
  }

  xMutex = xSemaphoreCreateMutex();
  if (xMutex == NULL)
  {
    printf("Failed to create mutex!\n");
  }

  // Create FreeRTOS tasks
  xTaskCreate(vTaskButtonLED, "ButtonLED", 256, NULL, 2, NULL);
  xTaskCreate(vTaskSync, "Sync", 256, NULL, 2, NULL);
  xTaskCreate(vTaskAsync, "Async", 256, NULL, 1, NULL);
  xTaskCreate(vTaskExtra, "Extra", 256, NULL, 1, NULL);

  printf("FreeRTOS tasks created.\n");

  // Start the scheduler
  vTaskStartScheduler();

  // Should never reach here
  printf("Scheduler failed to start!\n");
}

void loop()
{
  // loop() should never be reached
}
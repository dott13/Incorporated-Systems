#include <Arduino.h>
#include <stdio.h>
#include "led.h"
#include "logger.h"
#include "lab2tasks.h"

#define BUTTON_LED_PIN 2 // Task 1: Button to toggle LED (with internal pull-up)
#define BUTTON_INC_PIN 3 // Task 3: Button to increment variable
#define BUTTON_DEC_PIN 4 // Task 3: Button to decrement variable
#define LED_BLINK_PIN 12 // Task 2: LED for blinking

// STDIO redirection functions for Serial.
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
    ;
  }
  return Serial.read();
}

// Declare a FILE object for STDIO redirection.
static FILE serial_stream;

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }

  // Set up STDIO redirection.
  serial_stream.flags = _FDEV_SETUP_WRITE | _FDEV_SETUP_READ;
  serial_stream.put = serial_putchar;
  serial_stream.get = serial_getchar;
  stdout = &serial_stream;
  stdin = &serial_stream;

  // Initialize logger and LED.
  logger_init();
  led_init();

  // Configure pins for the tasks.
  pinMode(BUTTON_LED_PIN, INPUT_PULLUP); // Task 1 button (active LOW)
  pinMode(BUTTON_INC_PIN, INPUT_PULLUP); // Task 3 increment button
  pinMode(BUTTON_DEC_PIN, INPUT_PULLUP); // Task 3 decrement button
  pinMode(LED_BLINK_PIN, OUTPUT);        // Task 2 blinking LED

  printf("System initialized. Running sequential tasks...\n");
}

void loop()
{
  taskButtonLED();          // Task 1: Toggle LED on button press.
  taskBlinkLED();           // Task 2: Blink LED if Task 1 LED is off.
  taskVariableAdjustment(); // Task 3: Adjust state variable via buttons.
  taskIdle();               // Idle Task: Report system state.

  delay(50); // Delay to control task recurrence and reduce CPU load.
}

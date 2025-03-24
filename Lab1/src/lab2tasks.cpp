#include "lab2tasks.h"
#include "led.h"
#include "logger.h"
#include <Arduino.h>

#define BUTTON_LED_PIN 2 // Task 1: Button to toggle LED (with internal pull-up)
#define BUTTON_INC_PIN 3 // Task 3: Button to increment variable
#define BUTTON_DEC_PIN 4 // Task 3: Button to decrement variable
#define LED_BLINK_PIN 12 // Task 2: LED for blinking

// Global signals used for provider/consumer communication.
volatile bool buttonLEDState = false;       // Task 1 LED state (false = OFF, true = ON)
volatile unsigned long blinkDuration = 500; // Blink period for Task 2 LED in milliseconds
volatile int stateVariable = 0;             // State variable adjusted in Task 3

// Variables for button debouncing.
unsigned long lastDebounceTime_LED = 0;
unsigned long lastDebounceTime_Inc = 0;
unsigned long lastDebounceTime_Dec = 0;
const unsigned long debounceDelay = 50; // 50 ms debounce period

// Task 1: Check the button state and toggle the LED.
void taskButtonLED()
{
    bool currentButtonState = (digitalRead(BUTTON_LED_PIN) == LOW); // Active LOW
    unsigned long currentTime = millis();
    if (currentButtonState && (currentTime - lastDebounceTime_LED > debounceDelay))
    {
        buttonLEDState = !buttonLEDState;
        lastDebounceTime_LED = currentTime;
        if (buttonLEDState)
        {
            led_on();
            printf("Task1: LED toggled ON\n");
        }
        else
        {
            led_off();
            printf("Task1: LED toggled OFF\n");
        }
    }
}

// Task 2: Blink an LED if Task 1 LED is OFF.
void taskBlinkLED()
{
    static unsigned long previousBlinkTime = 0;
    static bool blinkState = false;
    unsigned long currentTime = millis();
    if (!buttonLEDState)
    { // Only blink when Task1 LED is OFF.
        if (currentTime - previousBlinkTime >= blinkDuration)
        {
            blinkState = !blinkState;
            digitalWrite(LED_BLINK_PIN, blinkState ? HIGH : LOW);
            previousBlinkTime = currentTime;
            printf("Task2: Blinking LED %s\n", blinkState ? "ON" : "OFF");
        }
    }
    else
    {
        // Ensure blinking LED is off when Task1 LED is ON.
        digitalWrite(LED_BLINK_PIN, LOW);
    }
}

// Task 3: Adjust a global state variable using two buttons.
void taskVariableAdjustment()
{
    bool incPressed = (digitalRead(BUTTON_INC_PIN) == LOW);
    bool decPressed = (digitalRead(BUTTON_DEC_PIN) == LOW);
    unsigned long currentTime = millis();

    if (incPressed && (currentTime - lastDebounceTime_Inc > debounceDelay))
    {
        stateVariable++;
        blinkDuration = 500 + stateVariable * 100; // Adjust blink period accordingly.
        lastDebounceTime_Inc = currentTime;
        printf("Task3: Variable incremented to %d, blinkDuration: %lu ms\n", stateVariable, blinkDuration);
    }

    if (decPressed && (currentTime - lastDebounceTime_Dec > debounceDelay))
    {
        stateVariable--;
        if (stateVariable < 0)
            stateVariable = 0;
        blinkDuration = 500 + stateVariable * 100;
        lastDebounceTime_Dec = currentTime;
        printf("Task3: Variable decremented to %d, blinkDuration: %lu ms\n", stateVariable, blinkDuration);
    }
}

// Idle Task: Reads the global signals and reports them.
void taskIdle()
{
    printf("Idle Task: LED state: %s, Blink Duration: %lu ms, Variable: %d\n",
           (buttonLEDState ? "ON" : "OFF"), blinkDuration, stateVariable);
}

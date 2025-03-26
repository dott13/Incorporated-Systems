#include "lab2tasks.h"
#include "led.h"
#include "logger.h"
#include <Arduino.h>

#define BUTTON_LED_PIN 2 // Task 1: Physical button on pin 2
#define BUTTON_INC_PIN 3 // Task 3: Physical button for increment
#define BUTTON_DEC_PIN 4 // Task 3: Physical button for decrement
#define LED_BLINK_PIN 12 // Task 2: External LED for blinking

// Global signals for provider/consumer communication.
volatile bool buttonLEDState = false;       // Task 1 LED state (false = OFF, true = ON)
volatile unsigned long blinkDuration = 500; // Blink period for Task 2 LED (in ms)
volatile int stateVariable = 0;             // State variable adjusted in Task 3

// Debounce timing variables.
unsigned long lastDebounceTime_LED = 0;
unsigned long lastDebounceTime_Inc = 0;
unsigned long lastDebounceTime_Dec = 0;
const unsigned long debounceDelay = 50; // 50 ms debounce period

// Task 1: Check physical button and Serial input for 't' to toggle the LED.
void taskButtonLED()
{
    bool physicalState = (digitalRead(BUTTON_LED_PIN) == LOW); // Physical button (active LOW)
    bool keyboardToggle = false;
    // Check for keyboard input.
    if (Serial.available() > 0)
    {
        char c = Serial.read();
        if (c == 't' || c == 'T')
        {
            keyboardToggle = true;
        }
    }

    unsigned long currentTime = millis();
    // If either physical button or keyboard command is detected...
    if ((physicalState || keyboardToggle) && (currentTime - lastDebounceTime_LED > debounceDelay))
    {
        buttonLEDState = !buttonLEDState;
        lastDebounceTime_LED = currentTime;
        if (buttonLEDState)
        {
            led_on();
            printf("Task1: LED toggled ON (by %s)\n", physicalState ? "button" : "keyboard");
        }
        else
        {
            led_off();
            printf("Task1: LED toggled OFF (by %s)\n", physicalState ? "button" : "keyboard");
        }
    }
}

// Task 2 remains unchanged.
void taskBlinkLED()
{
    static unsigned long previousBlinkTime = 0;
    static bool blinkState = false;
    unsigned long currentTime = millis();
    if (!buttonLEDState)
    { // Only blink when Task 1 LED is OFF.
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
        digitalWrite(LED_BLINK_PIN, LOW);
    }
}

// Task 3: Adjust state variable using both physical buttons and keyboard input.
// 'i' or 'I' increments, 'd' or 'D' decrements.
void taskVariableAdjustment()
{
    bool incPressed = (digitalRead(BUTTON_INC_PIN) == LOW);
    bool decPressed = (digitalRead(BUTTON_DEC_PIN) == LOW);
    bool keyboardInc = false;
    bool keyboardDec = false;

    // Check Serial input for increment ('i') or decrement ('d') commands.
    if (Serial.available() > 0)
    {
        char c = Serial.read();
        if (c == 'i' || c == 'I')
        {
            keyboardInc = true;
        }
        else if (c == 'd' || c == 'D')
        {
            keyboardDec = true;
        }
    }

    unsigned long currentTime = millis();

    if ((incPressed || keyboardInc) && (currentTime - lastDebounceTime_Inc > debounceDelay))
    {
        stateVariable++;
        blinkDuration = 500 + stateVariable * 100; // Adjust blink period accordingly.
        lastDebounceTime_Inc = currentTime;
        printf("Task3: Variable incremented to %d, blinkDuration: %lu ms (by %s)\n",
               stateVariable, blinkDuration, incPressed ? "button" : "keyboard");
    }

    if ((decPressed || keyboardDec) && (currentTime - lastDebounceTime_Dec > debounceDelay))
    {
        stateVariable--;
        if (stateVariable < 0)
            stateVariable = 0;
        blinkDuration = 500 + stateVariable * 100;
        lastDebounceTime_Dec = currentTime;
        printf("Task3: Variable decremented to %d, blinkDuration: %lu ms (by %s)\n",
               stateVariable, blinkDuration, decPressed ? "button" : "keyboard");
    }
}

// Idle Task: Report the current system state.
void taskIdle()
{
    printf("Idle Task: LED state: %s, Blink Duration: %lu ms, Variable: %d\n",
           (buttonLEDState ? "ON" : "OFF"), blinkDuration, stateVariable);
}

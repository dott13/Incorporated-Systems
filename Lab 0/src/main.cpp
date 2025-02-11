#include <Arduino.h>

// Define the pin numbers for the button and LED.
const int buttonPin = 2;  // Change to the pin you're using for the button
const int ledPin    = 13; // Change to the LED pin (13 is often the built-in LED on many boards)

// Variables to keep track of button state and LED state.
bool ledState = false;
bool lastButtonState = HIGH;  // Using INPUT_PULLUP, so default state is HIGH

// Variables for debouncing
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200; // milliseconds

void setup() {
  // Initialize the LED pin as an output.
  pinMode(ledPin, OUTPUT);
  // Initialize the button pin as an input with an internal pull-up resistor.
  pinMode(buttonPin, INPUT_PULLUP);

  // Optionally, start the serial monitor for debugging.
  Serial.begin(9600);
}

void loop() {
  // Read the current state of the button.
  bool reading = digitalRead(buttonPin);

  // Check if the button state has changed.
  if (reading != lastButtonState) {
    // Reset the debouncing timer.
    lastDebounceTime = millis();
  }

  // If the button state has been stable for longer than the debounce delay:
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Check for a button press (transition from HIGH to LOW)
    if (reading == LOW && lastButtonState == HIGH) {
      // Toggle the LED state.
      ledState = !ledState;
      digitalWrite(ledPin, ledState ? HIGH : LOW);

      // Optional: Print the new state to the serial monitor.
      Serial.print("LED is now ");
      Serial.println(ledState ? "ON" : "OFF");
    }
  }

  // Save the reading for the next loop iteration.
  lastButtonState = reading;
}

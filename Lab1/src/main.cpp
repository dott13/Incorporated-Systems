#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include "led.h"    // LED functions (led_init, led_on, led_off)
#include "logger.h" // Logger functions (log_info, log_success, log_error)
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// ===== LCD SETUP =====
// Create an I2C LCD object. Adjust the I2C address (0x27) as needed.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== KEYPAD SETUP =====
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {A0, A1, A2, A3}; // Change if needed
byte colPins[COLS] = {A4, A5, A6, A7}; // Change if needed
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ===== STDIO STREAMS =====

// (1) Combined output: write every character to both Serial and the LCD.
static int combined_putchar(char c, FILE *stream)
{
  if (c == '\n')
  {
    Serial.write('\r');
    // For LCD, we simply write the newline character.
  }
  Serial.write(c);
  lcd.write(c);
  return 0;
}

// (2) Keypad input: read a character from the keypad.
static int keypad_getchar(FILE *stream)
{
  char key = NO_KEY;
  // Wait for a key press
  while (key == NO_KEY)
  {
    key = keypad.getKey();
  }
  // For simplicity, we return the pressed key’s ASCII code.
  return key;
}

// Create FILE objects.
static FILE combined_stream; // for stdout
static FILE keypad_stream;   // for stdin

// The valid 4-digit code
const char *validCode = "1234";

void setup()
{
  // Initialize Serial.
  Serial.begin(9600);
  while (!Serial)
  {
    ; // Wait for Serial to initialize.
  }

  // Initialize LCD.
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Initialize LED and logger.
  led_init();
  logger_init();

  // Set up the combined output stream for stdout.
  combined_stream.flags = _FDEV_SETUP_WRITE;
  combined_stream.put = combined_putchar;
  stdout = &combined_stream; // All printf output goes here.

  // Set up the keypad input stream for stdin.
  keypad_stream.flags = _FDEV_SETUP_READ;
  keypad_stream.get = keypad_getchar;
  stdin = &keypad_stream; // All scanf input comes from the keypad.

  // Print startup message (will appear on both Serial and LCD).
  printf("System init.\nEnter 4-digit code:\n");
}

void loop()
{
  char codeBuffer[5] = {0}; // Buffer for 4 digits + null terminator

  // Use scanf to read a 4-character code from the keypad.
  // Note: This will wait until 4 non-whitespace characters are received.
  if (scanf("%4s", codeBuffer) > 0)
  {
    printf("\nCode entered: %s\n", codeBuffer);

    if (strcmp(codeBuffer, validCode) == 0)
    {
      printf("Access Granted\n");
      log_success("Access Granted");
      led_on(); // Keep LED on if correct.
      lcd.clear();
      lcd.print("Access Granted");
    }
    else
    {
      printf("Access Denied\n");
      log_error("Access Denied");
      lcd.clear();
      lcd.print("Access Denied");
      led_on();
      delay(1000);
      led_off();
    }

    delay(3000);

    // Prompt for next code.
    printf("\nEnter 4-digit code:\n");
    lcd.clear();
    lcd.print("Enter Code:");
  }
}

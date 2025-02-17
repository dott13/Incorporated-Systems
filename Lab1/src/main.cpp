#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include "led.h"
#include "logger.h"

// Custom putc function: writes a character to Serial.
static int serial_putchar(char c, FILE *stream) {
  if (c == '\n') {  // Insert carriage return before newline for proper terminal formatting.
    Serial.write('\r');
  }
  Serial.write(c);
  return 0;
}

// Custom getc function: reads a character from Serial.
static int serial_getchar(FILE *stream) {
  // Wait until a character is available.
  while (!Serial.available()) {
    ; // Busy-wait; consider a timeout if needed.
  }
  return Serial.read();
}

// Declare a static FILE object.
static FILE serial_stream;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for Serial to initialize.
  }
  
  // Manually initialize the FILE structure.
  // The member names (flags, put, get) are defined in avr-libc.
  serial_stream.flags  = _FDEV_SETUP_WRITE | _FDEV_SETUP_READ;
  serial_stream.put    = serial_putchar;
  serial_stream.get    = serial_getchar;
  
  // Redirect standard I/O to our custom serial stream.
  stdout = &serial_stream;
  stdin  = &serial_stream;
  
  // Initialize LED and logger.
  led_init();
  logger_init();
  
  log_info("System init. Write Commands...");
  log_info("Type 'on' to turn on the LED, 'off' to turn it off");
}

void loop() {
  char command[32];
  
  // Prompt the user.
  printf("Enter command: ");
  
  // Read a command using scanf.
  if (scanf("%31s", command) > 0) {
    // Process the command.
    if (strcmp(command, "on") == 0) {
      led_on();
      log_success("LED turned ON");
    } else if (strcmp(command, "off") == 0) {
      led_off();
      log_success("LED turned OFF");
    } else {
      log_error("Unknown command received");
      printf("Valid commands are: 'on' or 'off'\n");
    }
  }
}

#include <Arduino.h>
#include <led.h>
#include <logger.h>

void setup() {
  Serial.begin(9600);

  while(!Serial) {
    ;
  }

  led_init();
  logger_init();
  log_info("System init. Write Commands...");

  log_info("Type 'on' if you want to turn on the LED, 'off' to turn it off");
}

void loop() {
  if(Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

     if (input.equalsIgnoreCase("on")) {
      led_on();
      log_success("LED turned ON");
    }
    else if (input.equalsIgnoreCase("off")) {
      led_off();
      log_success("LED turned OFF");
    }
    else {
      log_error("Unknown command received: " + input);
      Serial.println("Valid commands are: 'on' or 'off'");
    }
  }
}

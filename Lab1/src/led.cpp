#include <led.h>

#define LED_PIN LED_BUILTIN

void led_init() {
    pinMode(LED_PIN, OUTPUT);
    led_off();
}

void led_on() {
    digitalWrite(LED_PIN, HIGH);
}

void led_off() {
    digitalWrite(LED_PIN, LOW);
}
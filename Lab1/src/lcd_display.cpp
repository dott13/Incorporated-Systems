#include "lcd_display.h"
#include <LiquidCrystal_I2C.h>
#include <stdarg.h>
#include <stdio.h>

static LiquidCrystal_I2C lcd(0x27, 16, 2);

void lcd_init()
{
    lcd.init();
    lcd.backlight();
}

void lcd_printf(const char *format, ...)
{
    char buffer[32];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    lcd.clear();
    lcd.print(buffer);
}
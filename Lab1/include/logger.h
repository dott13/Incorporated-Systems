#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

void logger_init();
void log_info(const String &message);
void log_success(const String &message);
void log_error(const String &message);


#endif
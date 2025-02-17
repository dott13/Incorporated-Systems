#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

void logger_init();
void log_info(const char *message);
void log_success(const char *message);
void log_error(const char *message);

#endif

#include "logger.h"
#include <stdio.h>

void logger_init() {
    // No additional initialization is needed for now.
}

void log_info(const char *message) {
    printf("[INFO] %s\n", message);
}

void log_success(const char *message) {
    printf("[SUCCESS] %s\n", message);
}

void log_error(const char *message) {
    printf("[ERROR] %s\n", message);
}

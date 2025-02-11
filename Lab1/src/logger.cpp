#include <logger.h>

void logger_init() {
    //No info is in here for now as we dont need initialization logic
}

void log_info(const String &message) {
    Serial.println("[INFO]" + message);
}

void log_success(const String &message) {
    Serial.println("[SUCCESS]" + message);
}

void log_error(const String &message) {
    Serial.println("[ERROR]" + message);
}
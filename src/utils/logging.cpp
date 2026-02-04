#include "logging.h"
#include <iostream>

namespace xerith {

void Logger::log(LogLevel level, const std::string& message) {
    std::string prefix;
    std::string color;

    switch (level) {
        case LogLevel::INFO:
            prefix = "[INFO]";
            color = "\033[32m"; // Green
            break;
        case LogLevel::DEBUG:
            prefix = "[DEBUG]";
            color = "\033[34m"; // Blue
            break;
        case LogLevel::WARNING:
            prefix = "[WARN]";
            color = "\033[33m"; // Yellow
            break;
        case LogLevel::ERROR:
            prefix = "[ERROR]";
            color = "\033[31m"; // Red
            break;
    }

    // Print: [LEVEL] Message (Reset Color)
    std::cout << color << prefix << " " << message << "\033[0m" << std::endl;
}

} // namespace xerith
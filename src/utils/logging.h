#ifndef XERITH_LOGGING_H
#define XERITH_LOGGING_H

#include <string>

namespace xerith {

enum class LogLevel {
    INFO,    // Green: Normal compiler progress
    DEBUG,   // Blue: Detailed internal data
    WARNING, // Yellow: Something suspicious but not fatal
    ERROR    // Red: A critical compiler failure
};

class Logger {
public:
    static void log(LogLevel level, const std::string& message);
    
    // helpers
    static void info(const std::string& msg) { log(LogLevel::INFO, msg); }
    static void debug(const std::string& msg) { log(LogLevel::DEBUG, msg); }
    static void warn(const std::string& msg) { log(LogLevel::WARNING, msg); }
    static void error(const std::string& msg) { log(LogLevel::ERROR, msg); }
};

} // namespace xerith

#endif // XERITH_LOGGING_H
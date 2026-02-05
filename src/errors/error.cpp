#include "error.h"
#include <sstream>

namespace xerith {

std::string get_severity_color(Severity severity) {
    switch (severity) {
        case Severity::Warning: return "\033[33m"; // Yellow
        case Severity::Error:   return "\033[31m"; // Red
        case Severity::Fatal:   return "\033[1;31m"; // Bold Red
        default:                return "\033[0m"; // Default
    }
}

std::string get_error_name(ErrorType type) {
    switch (type) {
        case ErrorType::Lexical:  return "Lexical Error";
        case ErrorType::Syntax:   return "Syntax Error";
        case ErrorType::Semantic: return "Semantic Error";
        case ErrorType::Type:     return "Type Error";
        case ErrorType::Internal: return "Internal Compiler Error";
        default:                  return "Error";
    }
}

/**
 * Turns the Error struct into a beautiful, human-readable string.
 * Format: [CODE] Type at file:line:col: Message
 */
std::string format_full_error(const Error& err) {
    std::stringstream ss;
    std::string color = get_severity_color(err.severity);
    std::string reset = "\033[0m";

    ss << color;
    
    // Add Error Code if available (e.g., [E001])
    if (!err.code.empty()) {
        ss << "[" << err.code << "] ";
    }

    // Add Error Type
    ss << get_error_name(err.type) << " ";

    // Add Location
    ss << "at " << err.location.filename << ":" 
       << err.location.line << ":" << err.location.column << ": ";

    // Add the actual message
    ss << reset << err.message;

    return ss.str();
}

} // namespace xerith
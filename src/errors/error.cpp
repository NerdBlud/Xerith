#include "error.h"
#include <sstream>

namespace xerith {

std::string get_severity_color(Severity severity) {
    switch (severity) {
        case Severity::Warning: return "\033[33m";
        case Severity::Error:   return "\033[31m";
        case Severity::Fatal:   return "\033[1;31m";
        default:                return "\033[0m";
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

std::string format_full_error(const Error& err) {
    std::stringstream ss;
    std::string color = get_severity_color(err.severity);
    std::string reset = "\033[0m";

    ss << color;
    if (!err.code.empty()) ss << "[" << err.code << "] ";
    ss << get_error_name(err.type) << " ";
    ss << "at " << err.location.filename << ":" 
       << err.location.line << ":" << err.location.column << ": ";
    ss << reset << err.message;

    return ss.str();
}

} // namespace xerith
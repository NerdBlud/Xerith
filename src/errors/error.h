#ifndef XERITH_ERROR_H
#define XERITH_ERROR_H

#include <string>
#include "../utils/span.h"

namespace xerith {

enum class ErrorType {
    Lexical,   // Typos in characters (e.g., unknown symbols)
    Syntax,    // Bad grammar (e.g., missing a semicolon)
    Semantic,  // Logic errors (e.g., using a variable before it exists)
    Type,      // Mismatched types (e.g., adding a string to a number)
    Internal   // The compiler itself crashed (should never happen!)
};

enum class Severity {
    Warning,   // Code will run, but might be buggy
    Error,     // Code will not compile
    Fatal      // Compiler must stop immediately
};

struct Error {
    ErrorType type;
    Severity severity;
    Span location;
    std::string message;
    std::string code; // Unique error code, e.g., "E001"

    Error(ErrorType t, Severity s, Span loc, std::string msg, std::string c = "")
        : type(t), severity(s), location(std::move(loc)), message(std::move(msg)), code(std::move(c)) {}
};

} // namespace xerith

#endif // XERITH_ERROR_H
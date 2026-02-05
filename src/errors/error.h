#ifndef XERITH_ERROR_H
#define XERITH_ERROR_H

#include <string>
#include "../utils/span.h"

namespace xerith {

enum class ErrorType {
    Lexical, Syntax, Semantic, Type, Internal
};

enum class Severity {
    Warning, Error, Fatal
};

struct Error {
    ErrorType type;
    Severity severity;
    Span location;
    std::string message;
    std::string code;

    Error(ErrorType t, Severity s, Span loc, std::string msg, std::string c = "")
        : type(t), severity(s), location(std::move(loc)), message(std::move(msg)), code(std::move(c)) {}
};

std::string get_error_name(ErrorType type);
std::string format_full_error(const Error& err);

} // namespace xerith

#endif // XERITH_ERROR_H
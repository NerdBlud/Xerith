#include "environment.h"

namespace xerith {

void Environment::define(const std::string& name, std::any value) {
    values[name] = value;
}

std::any Environment::get(const Token& name) {
    if (values.find(name.lexeme) != values.end()) {
        return values[name.lexeme];
    }
    throw std::runtime_error("Undefined variable '" + name.lexeme + "'.");
}

void Environment::assign(const Token& name, std::any value) {
    if (values.find(name.lexeme) != values.end()) {
        values[name.lexeme] = value;
        return;
    }
    throw std::runtime_error("Undefined variable '" + name.lexeme + "'.");
}

} // namespace xerith
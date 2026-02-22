#ifndef XERITH_ENVIRONMENT_H
#define XERITH_ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include <any>
#include <memory>
#include <stdexcept>
#include "../lexer/token.h"

namespace xerith {

class Environment : public std::enable_shared_from_this<Environment> {
public:
    std::shared_ptr<Environment> enclosing;
    std::unordered_map<std::string, std::any> values;

    // Constructors
    Environment() : enclosing(nullptr) {}
    Environment(std::shared_ptr<Environment> enclosing) : enclosing(enclosing) {}

    void define(const std::string& name, std::any value) {
        values[name] = std::move(value);
    }

    std::any get(const Token& name) {
        if (values.find(name.lexeme) != values.end()) {
            return values[name.lexeme];
        }

        if (enclosing != nullptr) return enclosing->get(name);

        throw std::runtime_error("Undefined variable '" + name.lexeme + "'.");
    }

    void assign(const Token& name, std::any value) {
        if (values.find(name.lexeme) != values.end()) {
            values[name.lexeme] = std::move(value);
            return;
        }

        if (enclosing != nullptr) {
            enclosing->assign(name, std::move(value));
            return;
        }

        throw std::runtime_error("Undefined variable '" + name.lexeme + "'.");
    }
};

} // namespace xerith

#endif
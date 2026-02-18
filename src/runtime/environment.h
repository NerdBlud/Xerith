#ifndef XERITH_ENVIRONMENT_H
#define XERITH_ENVIRONMENT_H

#include <string>
#include <map>
#include <any>
#include <stdexcept>
#include "../lexer/token.h"

namespace xerith {

class Environment {
public:
    void define(const std::string& name, std::any value);
    std::any get(const Token& name);
    void assign(const Token& name, std::any value);

private:
    std::map<std::string, std::any> values;
};

} // namespace xerith

#endif
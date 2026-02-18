#ifndef XERITH_INTERPRETER_H
#define XERITH_INTERPRETER_H

#include <vector>
#include <any>
#include "../parser/ast.h"
#include "environment.h"

namespace xerith {

class Interpreter {
public:
    void interpret(const std::vector<std::unique_ptr<Stmt>>& statements);

private:
    Environment environment;

    // Statement Execution
    void execute(Stmt* stmt);
    
    // Expression Evaluation
    std::any evaluate(Expr* expr);
    
    // Helpers
    bool is_truthy(std::any object);
    bool is_equal(std::any a, std::any b);
    std::string stringify(std::any object);
};

} // namespace xerith

#endif
#ifndef XERITH_AST_PRINTER_H
#define XERITH_AST_PRINTER_H

#include <string>
#include <vector>
#include <memory>
#include "ast.h"

namespace xerith {

class ASTPrinter {
public:
    std::string print(const std::vector<std::unique_ptr<Stmt>>& statements);
    std::string print(Expr* expr);

private:
    std::string print_stmt(Stmt* stmt);
    std::string parenthesize(const std::string& name, const std::vector<Expr*>& exprs);
};

} // namespace xerith

#endif
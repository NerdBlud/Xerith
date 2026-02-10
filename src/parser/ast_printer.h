#ifndef XERITH_AST_PRINTER_H
#define XERITH_AST_PRINTER_H

#include <string>
#include <vector>
#include <memory>
#include "ast.h"

namespace xerith {

class ASTPrinter {
public:
    std::string print(Expr* expr);

private:
    std::string parenthesize(const std::string& name, const std::vector<Expr*>& exprs);
};

} // namespace xerith

#endif
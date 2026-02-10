#include "ast_printer.h"
#include <sstream>

namespace xerith {

std::string ASTPrinter::print(Expr* expr) {
    if (!expr) return "nil";

    if (auto* e = dynamic_cast<BinaryExpr*>(expr)) {
        return parenthesize(e->op.lexeme, {e->left.get(), e->right.get()});
    }
    
    if (auto* e = dynamic_cast<GroupingExpr*>(expr)) {
        return parenthesize("group", {e->expression.get()});
    }
    
    if (auto* e = dynamic_cast<LiteralExpr*>(expr)) {
        return e->value.lexeme;
    }
    
    if (auto* e = dynamic_cast<UnaryExpr*>(expr)) {
        return parenthesize(e->op.lexeme, {e->right.get()});
    }

    return "?";
}

std::string ASTPrinter::parenthesize(const std::string& name, const std::vector<Expr*>& exprs) {
    std::stringstream ss;
    ss << "(" << name;
    for (auto* expr : exprs) {
        ss << " " << print(expr);
    }
    ss << ")";
    return ss.str();
}

} // namespace xerith
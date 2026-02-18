#include "ast_printer.h"
#include <sstream>

namespace xerith {

std::string ASTPrinter::print(const std::vector<std::unique_ptr<Stmt>>& statements) {
    std::stringstream ss;
    for (const auto& stmt : statements) {
        ss << print_stmt(stmt.get()) << "\n";
    }
    return ss.str();
}

std::string ASTPrinter::print_stmt(Stmt* stmt) {
    if (auto* s = dynamic_cast<VarStmt*>(stmt)) {
        std::string init = s->initializer ? print(s->initializer.get()) : "nil";
        return "(let " + s->name.lexeme + " " + init + ")";
    }
    if (auto* s = dynamic_cast<PrintStmt*>(stmt)) {
        return "(print " + print(s->expression.get()) + ")";
    }
    if (auto* s = dynamic_cast<ExpressionStmt*>(stmt)) {
        return "(stmt " + print(s->expression.get()) + ")";
    }
    return "(unknown stmt)";
}

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
    if (auto* e = dynamic_cast<VariableExpr*>(expr)) {
        return "(var " + e->name.lexeme + ")";
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
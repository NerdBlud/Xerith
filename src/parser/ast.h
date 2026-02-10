#ifndef XERITH_AST_H
#define XERITH_AST_H

#include <memory>
#include "../lexer/token.h"

namespace xerith {

struct Expr {
    virtual ~Expr() = default;
};

struct BinaryExpr : public Expr {
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

    BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right);
};

struct GroupingExpr : public Expr {
    std::unique_ptr<Expr> expression;

    GroupingExpr(std::unique_ptr<Expr> expression);
};

struct LiteralExpr : public Expr {
    Token value;
    LiteralExpr(Token value);
};

struct UnaryExpr : public Expr {
    Token op;
    std::unique_ptr<Expr> right;

    UnaryExpr(Token op, std::unique_ptr<Expr> right);
};

} // namespace xerith

#endif
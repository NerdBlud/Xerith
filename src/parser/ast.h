#ifndef XERITH_AST_H
#define XERITH_AST_H

#include <memory>
#include <vector>
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

struct VariableExpr : public Expr {
    Token name;
    VariableExpr(Token name);
};

struct Stmt {
    virtual ~Stmt() = default;
};

struct ExpressionStmt : public Stmt {
    std::unique_ptr<Expr> expression;
    ExpressionStmt(std::unique_ptr<Expr> expression);
};

struct PrintStmt : public Stmt {
    std::unique_ptr<Expr> expression;
    PrintStmt(std::unique_ptr<Expr> expression);
};

struct VarStmt : public Stmt {
    Token name;
    std::unique_ptr<Expr> initializer;
    VarStmt(Token name, std::unique_ptr<Expr> initializer);
};

} // namespace xerith

#endif
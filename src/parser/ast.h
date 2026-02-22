#ifndef XERITH_AST_H
#define XERITH_AST_H

#include <memory>
#include <vector>
#include <any>
#include "../lexer/token.h"

namespace xerith {

class BinaryExpr; class UnaryExpr; class LiteralExpr;
class GroupingExpr; class VariableExpr; class AssignExpr;

class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;
    virtual std::any visit_binary_expr(BinaryExpr& expr) = 0;
    virtual std::any visit_unary_expr(UnaryExpr& expr) = 0;
    virtual std::any visit_literal_expr(LiteralExpr& expr) = 0;
    virtual std::any visit_grouping_expr(GroupingExpr& expr) = 0;
    virtual std::any visit_variable_expr(VariableExpr& expr) = 0;
    virtual std::any visit_assign_expr(AssignExpr& expr) = 0;
};

class Expr {
public:
    virtual ~Expr() = default;
    virtual std::any accept(ExprVisitor& visitor) = 0;
};

class BinaryExpr : public Expr {
public:
    std::unique_ptr<Expr> left, right;
    Token op;
    BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}
    std::any accept(ExprVisitor& visitor) override { return visitor.visit_binary_expr(*this); }
};

class UnaryExpr : public Expr {
public:
    Token op;
    std::unique_ptr<Expr> right;
    UnaryExpr(Token op, std::unique_ptr<Expr> right) : op(std::move(op)), right(std::move(right)) {}
    std::any accept(ExprVisitor& visitor) override { return visitor.visit_unary_expr(*this); }
};

class LiteralExpr : public Expr {
public:
    Token value;
    LiteralExpr(Token value) : value(std::move(value)) {}
    std::any accept(ExprVisitor& visitor) override { return visitor.visit_literal_expr(*this); }
};

class GroupingExpr : public Expr {
public:
    std::unique_ptr<Expr> expression;
    GroupingExpr(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
    std::any accept(ExprVisitor& visitor) override { return visitor.visit_grouping_expr(*this); }
};

class VariableExpr : public Expr {
public:
    Token name;
    VariableExpr(Token name) : name(std::move(name)) {}
    std::any accept(ExprVisitor& visitor) override { return visitor.visit_variable_expr(*this); }
};

class AssignExpr : public Expr {
public:
    Token name;
    std::unique_ptr<Expr> value;
    AssignExpr(Token name, std::unique_ptr<Expr> value) : name(std::move(name)), value(std::move(value)) {}
    std::any accept(ExprVisitor& visitor) override { return visitor.visit_assign_expr(*this); }
};

class PrintStmt; class ExpressionStmt; class VarStmt;
class BlockStmt; class WhileStmt; class IfStmt;

class StmtVisitor {
public:
    virtual ~StmtVisitor() = default;
    virtual std::any visit_print_stmt(PrintStmt& stmt) = 0;
    virtual std::any visit_expression_stmt(ExpressionStmt& stmt) = 0;
    virtual std::any visit_var_stmt(VarStmt& stmt) = 0;
    virtual std::any visit_block_stmt(BlockStmt& stmt) = 0;
    virtual std::any visit_while_stmt(WhileStmt& stmt) = 0;
    virtual std::any visit_if_stmt(IfStmt& stmt) = 0;
};

class Stmt {
public:
    virtual ~Stmt() = default;
    virtual std::any accept(StmtVisitor& visitor) = 0;
};

class PrintStmt : public Stmt {
public:
    std::unique_ptr<Expr> expression;
    PrintStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
    std::any accept(StmtVisitor& visitor) override { return visitor.visit_print_stmt(*this); }
};

class ExpressionStmt : public Stmt {
public:
    std::unique_ptr<Expr> expression;
    ExpressionStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
    std::any accept(StmtVisitor& visitor) override { return visitor.visit_expression_stmt(*this); }
};

class VarStmt : public Stmt {
public:
    Token name;
    std::unique_ptr<Expr> initializer;
    VarStmt(Token name, std::unique_ptr<Expr> initializer)
        : name(std::move(name)), initializer(std::move(initializer)) {}
    std::any accept(StmtVisitor& visitor) override { return visitor.visit_var_stmt(*this); }
};

class BlockStmt : public Stmt {
public:
    std::vector<std::unique_ptr<Stmt>> statements;
    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements) : statements(std::move(statements)) {}
    std::any accept(StmtVisitor& visitor) override { return visitor.visit_block_stmt(*this); }
};

class WhileStmt : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
        : condition(std::move(condition)), body(std::move(body)) {}
    std::any accept(StmtVisitor& visitor) override { return visitor.visit_while_stmt(*this); }
};

class IfStmt : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> then_branch;
    std::unique_ptr<Stmt> else_branch;
    IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then_branch, std::unique_ptr<Stmt> else_branch)
        : condition(std::move(condition)), then_branch(std::move(then_branch)), else_branch(std::move(else_branch)) {}
    std::any accept(StmtVisitor& visitor) override { return visitor.visit_if_stmt(*this); }
};

} 
#endif
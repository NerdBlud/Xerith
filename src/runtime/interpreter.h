#ifndef XERITH_INTERPRETER_H
#define XERITH_INTERPRETER_H

#include <memory>
#include <vector>
#include <any>
#include "../parser/ast.h"
#include "environment.h"

namespace xerith {

class Interpreter : public ExprVisitor, public StmtVisitor {
public:
    Interpreter();
    void interpret(const std::vector<std::unique_ptr<Stmt>>& statements);

    // Stmt Visitor Methods
    std::any visit_print_stmt(PrintStmt& stmt) override;
    std::any visit_expression_stmt(ExpressionStmt& stmt) override;
    std::any visit_var_stmt(VarStmt& stmt) override;
    std::any visit_block_stmt(BlockStmt& stmt) override;
    std::any visit_while_stmt(WhileStmt& stmt) override;
    std::any visit_if_stmt(IfStmt& stmt) override;

    // Expr Visitor Methods
    std::any visit_binary_expr(BinaryExpr& expr) override;
    std::any visit_unary_expr(UnaryExpr& expr) override;
    std::any visit_literal_expr(LiteralExpr& expr) override;
    std::any visit_grouping_expr(GroupingExpr& expr) override;
    std::any visit_variable_expr(VariableExpr& expr) override;
    std::any visit_assign_expr(AssignExpr& expr) override;

    // Execution Helpers
    void execute_block(const std::vector<std::unique_ptr<Stmt>>& statements, 
                       std::shared_ptr<Environment> inner_env);

private:
    std::shared_ptr<Environment> environment;
    
    void execute(Stmt& stmt);
    std::any evaluate(Expr& expr);
    
    // Evaluation Helpers
    bool is_truthy(const std::any& value);
    bool is_equal(const std::any& a, const std::any& b);
};

} // namespace

#endif
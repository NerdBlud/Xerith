#include "interpreter.h"
#include <iostream>

namespace xerith {

Interpreter::Interpreter() : environment(std::make_shared<Environment>()) {}

void Interpreter::interpret(const std::vector<std::unique_ptr<Stmt>>& statements) {
    try {
        for (const auto& statement : statements) {
            execute(*statement);
        }
    } catch (const std::runtime_error& error) {
        std::cerr << "Runtime Error: " << error.what() << std::endl;
    }
}

void Interpreter::execute(Stmt& stmt) { stmt.accept(*this); }
std::any Interpreter::evaluate(Expr& expr) { return expr.accept(*this); }

bool Interpreter::is_truthy(const std::any& value) {
    if (!value.has_value()) return false;
    if (value.type() == typeid(bool)) return std::any_cast<bool>(value);
    return true;
}

bool Interpreter::is_equal(const std::any& a, const std::any& b) {
    if (a.type() != b.type()) return false;
    if (a.type() == typeid(double)) return std::any_cast<double>(a) == std::any_cast<double>(b);
    if (a.type() == typeid(bool)) return std::any_cast<bool>(a) == std::any_cast<bool>(b);
    if (a.type() == typeid(std::string)) return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
    return false;
}

void Interpreter::execute_block(const std::vector<std::unique_ptr<Stmt>>& statements, std::shared_ptr<Environment> inner_env) {
    std::shared_ptr<Environment> previous = this->environment;
    try {
        this->environment = inner_env;
        for (const auto& statement : statements) execute(*statement);
    } catch (...) {
        this->environment = previous;
        throw;
    }
    this->environment = previous;
}

std::any Interpreter::visit_if_stmt(IfStmt& stmt) {
    if (is_truthy(evaluate(*stmt.condition))) {
        execute(*stmt.then_branch);
    } else if (stmt.else_branch != nullptr) {
        execute(*stmt.else_branch);
    }
    return {};
}

std::any Interpreter::visit_while_stmt(WhileStmt& stmt) {
    while (is_truthy(evaluate(*stmt.condition))) {
        execute(*stmt.body);
    }
    return {};
}

std::any Interpreter::visit_block_stmt(BlockStmt& stmt) {
    execute_block(stmt.statements, std::make_shared<Environment>(environment));
    return {};
}

std::any Interpreter::visit_var_stmt(VarStmt& stmt) {
    std::any value;
    if (stmt.initializer != nullptr) value = evaluate(*stmt.initializer);
    environment->define(stmt.name.lexeme, value);
    return {};
}

std::any Interpreter::visit_print_stmt(PrintStmt& stmt) {
    std::any value = evaluate(*stmt.expression);
    if (value.type() == typeid(double)) std::cout << std::any_cast<double>(value) << std::endl;
    else if (value.type() == typeid(std::string)) std::cout << std::any_cast<std::string>(value) << std::endl;
    else if (value.type() == typeid(bool)) std::cout << (std::any_cast<bool>(value) ? "true" : "false") << std::endl;
    else std::cout << "nil" << std::endl;
    return {};
}

std::any Interpreter::visit_expression_stmt(ExpressionStmt& stmt) {
    evaluate(*stmt.expression);
    return {};
}

std::any Interpreter::visit_variable_expr(VariableExpr& expr) { return environment->get(expr.name); }

std::any Interpreter::visit_assign_expr(AssignExpr& expr) {
    std::any value = evaluate(*expr.value);
    environment->assign(expr.name, value);
    return value;
}

std::any Interpreter::visit_literal_expr(LiteralExpr& expr) {
    if (expr.value.type == TokenType::NUMBER) return std::stod(expr.value.lexeme);
    if (expr.value.type == TokenType::STRING) return expr.value.lexeme;
    if (expr.value.type == TokenType::TRUE) return true;
    if (expr.value.type == TokenType::FALSE) return false;
    return std::any();
}

std::any Interpreter::visit_grouping_expr(GroupingExpr& expr) { return evaluate(*expr.expression); }

std::any Interpreter::visit_unary_expr(UnaryExpr& expr) {
    std::any right = evaluate(*expr.right);
    if (expr.op.type == TokenType::MINUS) return -std::any_cast<double>(right);
    if (expr.op.type == TokenType::BANG) return !is_truthy(right);
    return std::any();
}

std::any Interpreter::visit_binary_expr(BinaryExpr& expr) {
    std::any left = evaluate(*expr.left);
    std::any right = evaluate(*expr.right);
    switch (expr.op.type) {
        case TokenType::PLUS:
            if (left.type() == typeid(double)) return std::any_cast<double>(left) + std::any_cast<double>(right);
            if (left.type() == typeid(std::string)) return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
            break;
        case TokenType::MINUS: return std::any_cast<double>(left) - std::any_cast<double>(right);
        case TokenType::STAR: return std::any_cast<double>(left) * std::any_cast<double>(right);
        case TokenType::SLASH: return std::any_cast<double>(left) / std::any_cast<double>(right);
        case TokenType::GREATER: return std::any_cast<double>(left) > std::any_cast<double>(right);
        case TokenType::GREATER_EQUAL: return std::any_cast<double>(left) >= std::any_cast<double>(right);
        case TokenType::LESS: return std::any_cast<double>(left) < std::any_cast<double>(right);
        case TokenType::LESS_EQUAL: return std::any_cast<double>(left) <= std::any_cast<double>(right);
        case TokenType::EQUAL_EQUAL: return is_equal(left, right);
        case TokenType::BANG_EQUAL: return !is_equal(left, right);
        default: break;
    }
    return std::any();
}

}
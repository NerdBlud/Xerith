#include "interpreter.h"
#include <iostream>
#include <string>

namespace xerith {

void Interpreter::interpret(const std::vector<std::unique_ptr<Stmt>>& statements) {
    try {
        for (const auto& stmt : statements) {
            execute(stmt.get());
        }
    } catch (const std::runtime_error& error) {
        std::cerr << "[Runtime Error] " << error.what() << std::endl;
    }
}

// --- Statement Execution ---

void Interpreter::execute(Stmt* stmt) {
    if (auto* s = dynamic_cast<PrintStmt*>(stmt)) {
        std::any value = evaluate(s->expression.get());
        std::cout << stringify(value) << std::endl;
    } 
    else if (auto* s = dynamic_cast<VarStmt*>(stmt)) {
        std::any value = nullptr;
        if (s->initializer != nullptr) {
            value = evaluate(s->initializer.get());
        }
        environment.define(s->name.lexeme, value);
    } 
    else if (auto* s = dynamic_cast<ExpressionStmt*>(stmt)) {
        evaluate(s->expression.get());
    }
}

// --- Expression Evaluation ---

std::any Interpreter::evaluate(Expr* expr) {
    if (auto* e = dynamic_cast<LiteralExpr*>(expr)) {
        // Convert token lexeme to double if it's a number
        if (e->value.type == TokenType::NUMBER) return std::stod(e->value.lexeme);
        if (e->value.type == TokenType::STRING) return e->value.lexeme; // Already stripped quotes in Lexer?
        if (e->value.type == TokenType::TRUE) return true;
        if (e->value.type == TokenType::FALSE) return false;
        return nullptr;
    }

    if (auto* e = dynamic_cast<VariableExpr*>(expr)) {
        return environment.get(e->name);
    }

    if (auto* e = dynamic_cast<GroupingExpr*>(expr)) {
        return evaluate(e->expression.get());
    }

    if (auto* e = dynamic_cast<UnaryExpr*>(expr)) {
        std::any right = evaluate(e->right.get());
        switch (e->op.type) {
            case TokenType::MINUS: return -std::any_cast<double>(right);
            case TokenType::BANG: return !is_truthy(right);
            default: break;
        }
    }

    if (auto* e = dynamic_cast<BinaryExpr*>(expr)) {
        std::any left = evaluate(e->left.get());
        std::any right = evaluate(e->right.get());

        switch (e->op.type) {
            // Arithmetic
            case TokenType::PLUS:
                if (left.type() == typeid(double) && right.type() == typeid(double))
                    return std::any_cast<double>(left) + std::any_cast<double>(right);
                if (left.type() == typeid(std::string) && right.type() == typeid(std::string))
                    return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
                throw std::runtime_error("Operands must be two numbers or two strings.");
            case TokenType::MINUS: return std::any_cast<double>(left) - std::any_cast<double>(right);
            case TokenType::STAR:  return std::any_cast<double>(left) * std::any_cast<double>(right);
            case TokenType::SLASH: return std::any_cast<double>(left) / std::any_cast<double>(right);

            // Comparison
            case TokenType::GREATER: return std::any_cast<double>(left) > std::any_cast<double>(right);
            case TokenType::GREATER_EQUAL: return std::any_cast<double>(left) >= std::any_cast<double>(right);
            case TokenType::LESS: return std::any_cast<double>(left) < std::any_cast<double>(right);
            case TokenType::LESS_EQUAL: return std::any_cast<double>(left) <= std::any_cast<double>(right);
            
            // Equality
            case TokenType::EQUAL_EQUAL: return is_equal(left, right);
            case TokenType::BANG_EQUAL: return !is_equal(left, right);

            default: break;
        }
    }

    return nullptr;
}

// --- Helpers ---

bool Interpreter::is_truthy(std::any object) {
    if (object.type() == typeid(nullptr)) return false;
    if (object.type() == typeid(bool)) return std::any_cast<bool>(object);
    return true;
}

bool Interpreter::is_equal(std::any a, std::any b) {
    if (a.type() == typeid(nullptr) && b.type() == typeid(nullptr)) return true;
    if (a.type() == typeid(nullptr)) return false;
    
    if (a.type() == typeid(double) && b.type() == typeid(double))
        return std::any_cast<double>(a) == std::any_cast<double>(b);
    if (a.type() == typeid(std::string) && b.type() == typeid(std::string))
        return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
    if (a.type() == typeid(bool) && b.type() == typeid(bool))
        return std::any_cast<bool>(a) == std::any_cast<bool>(b);

    return false;
}

std::string Interpreter::stringify(std::any object) {
    if (object.type() == typeid(nullptr)) return "nil";
    if (object.type() == typeid(double)) {
        std::string text = std::to_string(std::any_cast<double>(object));
        // Remove trailing zeros for clean output (e.g., 10.000000 -> 10)
        if (text.find('.') != std::string::npos) {
            text.erase(text.find_last_not_of('0') + 1, std::string::npos);
            if (text.back() == '.') text.pop_back();
        }
        return text;
    }
    if (object.type() == typeid(bool)) return std::any_cast<bool>(object) ? "true" : "false";
    if (object.type() == typeid(std::string)) return std::any_cast<std::string>(object);
    return "[unknown type]";
}

} // namespace xerith
#include "parser.h"
#include <stdexcept>
#include <iostream>

namespace xerith {

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!is_at_end()) {
        auto decl = declaration();
        if (decl) statements.push_back(std::move(decl));
    }
    return statements;
}

std::unique_ptr<Stmt> Parser::declaration() {
    try {
        if (match({TokenType::LET})) return var_declaration();
        return statement();
    } catch (const std::runtime_error& error) {
        std::cerr << "[Parse Error] " << error.what() << std::endl;
        synchronize();
        return nullptr;
    }
}

std::unique_ptr<Stmt> Parser::var_declaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    std::unique_ptr<Expr> initializer = nullptr;
    if (match({TokenType::EQUAL})) initializer = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<VarStmt>(name, std::move(initializer));
}

std::unique_ptr<Stmt> Parser::statement() {
    if (match({TokenType::IF})) return if_statement();
    if (match({TokenType::FOR})) return for_statement();
    if (match({TokenType::PRINT})) return print_statement();
    if (match({TokenType::WHILE})) return while_statement();
    if (match({TokenType::LEFT_BRACE})) return std::make_unique<BlockStmt>(block());
    return expression_statement();
}

std::unique_ptr<Stmt> Parser::if_statement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

    auto then_branch = statement();
    std::unique_ptr<Stmt> else_branch = nullptr;
    if (match({TokenType::ELSE})) {
        else_branch = statement();
    }

    return std::make_unique<IfStmt>(std::move(condition), std::move(then_branch), std::move(else_branch));
}

std::unique_ptr<Stmt> Parser::for_statement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");
    std::unique_ptr<Stmt> initializer;
    if (match({TokenType::SEMICOLON})) initializer = nullptr;
    else if (match({TokenType::LET})) initializer = var_declaration();
    else initializer = expression_statement();

    std::unique_ptr<Expr> condition = nullptr;
    if (!check(TokenType::SEMICOLON)) condition = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

    std::unique_ptr<Expr> increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN)) increment = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

    std::unique_ptr<Stmt> body = statement();
    if (increment != nullptr) {
        std::vector<std::unique_ptr<Stmt>> body_stmts;
        body_stmts.push_back(std::move(body));
        body_stmts.push_back(std::make_unique<ExpressionStmt>(std::move(increment)));
        body = std::make_unique<BlockStmt>(std::move(body_stmts));
    }
    if (condition == nullptr) condition = std::make_unique<LiteralExpr>(Token(TokenType::TRUE, "true", previous().span));
    body = std::make_unique<WhileStmt>(std::move(condition), std::move(body));
    if (initializer != nullptr) {
        std::vector<std::unique_ptr<Stmt>> wrapper;
        wrapper.push_back(std::move(initializer));
        wrapper.push_back(std::move(body));
        body = std::make_unique<BlockStmt>(std::move(wrapper));
    }
    return body;
}

std::unique_ptr<Stmt> Parser::while_statement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    auto body = statement();
    return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
}

std::unique_ptr<Stmt> Parser::print_statement() {
    auto value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_unique<PrintStmt>(std::move(value));
}

std::unique_ptr<Stmt> Parser::expression_statement() {
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_unique<ExpressionStmt>(std::move(expr));
}

std::vector<std::unique_ptr<Stmt>> Parser::block() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!check(TokenType::RIGHT_BRACE) && !is_at_end()) {
        statements.push_back(declaration());
    }
    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

std::unique_ptr<Expr> Parser::expression() { return assignment(); }

std::unique_ptr<Expr> Parser::assignment() {
    auto expr = equality();
    if (match({TokenType::EQUAL})) {
        Token equals = previous();
        auto value = assignment();
        if (VariableExpr* v = dynamic_cast<VariableExpr*>(expr.get())) {
            return std::make_unique<AssignExpr>(v->name, std::move(value));
        }
        throw std::runtime_error("Invalid assignment target.");
    }
    return expr;
}

std::unique_ptr<Expr> Parser::equality() {
    auto expr = comparison();
    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        auto right = comparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
    auto expr = term();
    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token op = previous();
        auto right = term();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::term() {
    auto expr = factor();
    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token op = previous();
        auto right = factor();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    auto expr = unary();
    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token op = previous();
        auto right = unary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        auto right = unary();
        return std::make_unique<UnaryExpr>(op, std::move(right));
    }
    return primary();
}

std::unique_ptr<Expr> Parser::primary() {
    if (match({TokenType::FALSE})) return std::make_unique<LiteralExpr>(previous());
    if (match({TokenType::TRUE})) return std::make_unique<LiteralExpr>(previous());
    if (match({TokenType::NIL})) return std::make_unique<LiteralExpr>(previous());
    if (match({TokenType::NUMBER, TokenType::STRING})) return std::make_unique<LiteralExpr>(previous());
    if (match({TokenType::IDENTIFIER})) return std::make_unique<VariableExpr>(previous());
    if (match({TokenType::LEFT_PAREN})) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<GroupingExpr>(std::move(expr));
    }
    throw std::runtime_error("Expect expression.");
}

bool Parser::match(const std::vector<TokenType>& types) {
    for (auto type : types) { if (check(type)) { advance(); return true; } }
    return false;
}

bool Parser::check(TokenType type) const { return !is_at_end() && peek().type == type; }
Token Parser::advance() { if (!is_at_end()) current++; return previous(); }
bool Parser::is_at_end() const { return peek().type == TokenType::END_OF_FILE; }
Token Parser::peek() const { return tokens[current]; }
Token Parser::previous() const { return tokens[current - 1]; }
Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw std::runtime_error(message);
}

void Parser::synchronize() {
    advance();
    while (!is_at_end()) {
        if (previous().type == TokenType::SEMICOLON) return;
        switch (peek().type) {
            case TokenType::CLASS: case TokenType::FUN: case TokenType::LET:
            case TokenType::FOR: case TokenType::IF: case TokenType::WHILE:
            case TokenType::PRINT: case TokenType::RETURN: return;
            default: break;
        }
        advance();
    }
}

}
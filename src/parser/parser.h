#ifndef XERITH_PARSER_H
#define XERITH_PARSER_H

#include <vector>
#include <memory>
#include <string>
#include "../lexer/token.h"
#include "ast.h"

namespace xerith {

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    
    // The entry point: returns a list of statements to execute
    std::vector<std::unique_ptr<Stmt>> parse();

private:
    // --- Statement & Declaration Rules ---
    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> var_declaration();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> print_statement();
    std::unique_ptr<Stmt> expression_statement();

    // --- Expression Rules (Precedence Order) ---
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();

    // --- Navigation & Error Helpers ---
    bool match(const std::vector<TokenType>& types);
    bool check(TokenType type) const;
    Token advance();
    bool is_at_end() const;
    Token peek() const;
    Token previous() const;
    Token consume(TokenType type, const std::string& message);
    void synchronize();

    const std::vector<Token>& tokens;
    int current = 0;
};

} // namespace xerith

#endif
#ifndef XERITH_TOKEN_H
#define XERITH_TOKEN_H

#include <string>
#include "../utils/span.h"

namespace xerith {

enum class TokenType {
    // Single-character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals
    IDENTIFIER, STRING, NUMBER,

    // Keywords
    AND, CLASS, ELSE, FALSE, FN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, LET, WHILE,

    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string lexeme; 
    Span span;        

    Token(TokenType t, std::string lex, Span s)
        : type(t), lexeme(std::move(lex)), span(std::move(s)) {}
};

} // namespace xerith

#endif // XERITH_TOKEN_H
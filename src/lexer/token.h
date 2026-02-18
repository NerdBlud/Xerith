#ifndef XERITH_TOKEN_H
#define XERITH_TOKEN_H

#include <string>
#include "../errors/diagnostics.h" 

namespace xerith {

enum class TokenType {
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
    IDENTIFIER, STRING, NUMBER,
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, LET, WHILE, FN,
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string lexeme;
    Span span; 

    Token(TokenType type, std::string lexeme, Span span)
        : type(type), lexeme(std::move(lexeme)), span(std::move(span)) {}
};

} // namespace xerith

#endif
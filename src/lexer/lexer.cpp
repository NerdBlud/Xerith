#include "lexer.h"
#include "../errors/diagnostics.h"

namespace xerith {

Lexer::Lexer(std::string source, std::string filename) 
    : source(std::move(source)), filename(std::move(filename)) {}

bool Lexer::is_at_end() const {
    return current >= source.length();
}

char Lexer::advance() {
    column++;
    return source[current++];
}

void Lexer::add_token(TokenType type) {
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, Span(filename, line, column - (current - start)));
}

std::vector<Token> Lexer::scan_tokens() {
    while (!is_at_end()) {
        start = current;
        scan_token();
    }

    tokens.emplace_back(TokenType::END_OF_FILE, "", Span(filename, line, column));
    return tokens;
}

void Lexer::scan_token() {
    char c = advance();
    switch (c) {
        case '(': add_token(TokenType::LEFT_PAREN); break;
        case ')': add_token(TokenType::RIGHT_PAREN); break;
        case '{': add_token(TokenType::LEFT_BRACE); break;
        case '}': add_token(TokenType::RIGHT_BRACE); break;
        case ',': add_token(TokenType::COMMA); break;
        case '.': add_token(TokenType::DOT); break;
        case '-': add_token(TokenType::MINUS); break;
        case '+': add_token(TokenType::PLUS); break;
        case ';': add_token(TokenType::SEMICOLON); break;
        case '*': add_token(TokenType::STAR); break;
        
        // Handle whitespace
        case ' ':
        case '\r':
        case '\t':
            break;

        case '\n':
            line++;
            column = 1;
            break;

        default:
            Diagnostics::report(Error(
                ErrorType::Lexical,
                Severity::Error,
                Span(filename, line, column - 1),
                "Unexpected character: " + std::string(1, c)
            ));
            break;
    }
}

} // namespace xerith
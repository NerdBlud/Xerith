#include "lexer.h"
#include "../errors/diagnostics.h"
#include <unordered_map>

namespace xerith {

static const std::unordered_map<std::string, TokenType> keywords = {
    {"and",    TokenType::AND},
    {"class",  TokenType::CLASS},
    {"else",   TokenType::ELSE},
    {"false",  TokenType::FALSE},
    {"fn",     TokenType::FN},
    {"fun",    TokenType::FUN},
    {"for",    TokenType::FOR},
    {"if",     TokenType::IF},
    {"nil",    TokenType::NIL},
    {"or",     TokenType::OR},
    {"return", TokenType::RETURN},
    {"let",    TokenType::LET},
    {"while",  TokenType::WHILE},
    {"print",  TokenType::PRINT},
};

Lexer::Lexer(std::string source, std::string filename) 
    : source(std::move(source)), filename(std::move(filename)) {}

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

        case '!': add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case '=': add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
        case '<': add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case '>': add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;

        case '/':
            if (match('/')) {
                // A comment goes until the end of the line.
                while (peek() != '\n' && !is_at_end()) advance();
            } else {
                add_token(TokenType::SLASH);
            }
            break;

        case ' ':
        case '\r':
        case '\t':
            break;

        case '\n':
            line++;
            column = 1;
            break;

        case '"': string(); break;

        default:
            if (isdigit(c)) {
                number();
            } else if (isalpha(c) || c == '_') {
                identifier();
            } else {
                Diagnostics::report(Error(
                    ErrorType::Lexical,
                    Severity::Error,
                    Span(filename, line, column - 1),
                    "Unexpected character: " + std::string(1, c)
                ));
            }
            break;
    }
}

void Lexer::string() {
    while (peek() != '"' && !is_at_end()) {
        if (peek() == '\n') { line++; column = 1; }
        advance();
    }

    if (is_at_end()) {
        Diagnostics::report(Error(ErrorType::Lexical, Severity::Error, 
            Span(filename, line, column), "Unterminated string."));
        return;
    }

    // The closing ".
    advance(); 

    std::string value = source.substr(start + 1, current - start - 2);
    tokens.emplace_back(TokenType::STRING, value, Span(filename, line, column - (current - start)));
}

void Lexer::number() {
    while (isdigit(peek())) advance();

     if (peek() == '.' && isdigit(peek_next())) {
        // Consume the "."
        advance();
        while (isdigit(peek())) advance();
    }

    add_token(TokenType::NUMBER);
}

void Lexer::identifier() {
    while (isalnum(peek()) || peek() == '_') advance();

    std::string text = source.substr(start, current - start);
    auto it = keywords.find(text);
    TokenType type = (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;

    add_token(type); 
}

bool Lexer::is_at_end() const {
    return current >= (int)source.length();
}

char Lexer::advance() {
    column++;
    return source[current++];
}

char Lexer::peek() const {
    if (is_at_end()) return '\0';
    return source[current];
}

char Lexer::peek_next() const {
    if (current + 1 >= (int)source.length()) return '\0';
    return source[current + 1];
}

bool Lexer::match(char expected) {
    if (is_at_end()) return false;
    if (source[current] != expected) return false;

    current++;
    column++;
    return true;
}

void Lexer::add_token(TokenType type) {
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, Span(filename, line, column - (current - start)));
}

} // namespace xerith
#ifndef XERITH_LEXER_H
#define XERITH_LEXER_H

#include <string>
#include <vector>
#include "token.h"

namespace xerith {

class Lexer {
public:
    Lexer(std::string source, std::string filename);
    std::vector<Token> scan_tokens();

private:
    // --- Existing Helpers ---
    bool is_at_end() const;
    char advance();
    void add_token(TokenType type);
    void scan_token();

    // --- NEW Helpers (Add these!) ---
    char peek() const;
    bool match(char expected);
    void string();
    void number();
    void identifier();

    std::string source;
    std::string filename;
    std::vector<Token> tokens;

    int start = 0;
    int current = 0;
    int line = 1;
    int column = 1;
};

} // namespace xerith

#endif
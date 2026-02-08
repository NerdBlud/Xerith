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
    bool is_at_end() const;
    char advance();
    void add_token(TokenType type);
    void scan_token();

    std::string source;
    std::string filename;
    std::vector<Token> tokens;

    int start = 0;   // First character in the lexeme being scanned
    int current = 0; // Current character being considered
    int line = 1;    // Current line for the Span
    int column = 1;  // Current column for the Span
};

} // namespace xerith

#endif // XERITH_LEXER_H
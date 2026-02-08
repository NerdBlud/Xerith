#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "utils/logging.h"
#include "lexer/lexer.h"
#include "lexer/token.h"

using namespace xerith;

std::string read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        Logger::error("Could not open file: " + path);
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: xerith [file.xrth]" << std::endl;
        return 64;
    }

    std::string filename = argv[1];
    Logger::info("Compiling: " + filename);

    std::string source = read_file(filename);

    Lexer lexer(source, filename);
    std::vector<Token> tokens = lexer.scan_tokens();

std::cout << "\n--- SCAN RESULTS ---\n";
    for (const auto& token : tokens) {
        std::cout << "[Line " << token.span.line << "] " 
        << xerith::to_string(token.type) << " : '" << token.lexeme << "'\n";
    }

    return 0;
}
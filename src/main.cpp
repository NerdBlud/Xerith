#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/ast_printer.h"
#include "runtime/interpreter.h"

using namespace xerith;

void run(const std::string& source, const std::string& filename) {
    if (source.empty()) return;

    // 1. Lexing
    Lexer lexer(source, filename);
    std::vector<Token> tokens = lexer.scan_tokens();
    
    // 2. Parsing
    Parser parser(tokens);
    try {
        std::vector<std::unique_ptr<Stmt>> statements = parser.parse();

        if (statements.empty()) {
            // If we have tokens but no statements, there's a syntax mismatch
            if (tokens.size() > 1) {
                std::cerr << "[Parser] Failed to generate statements from tokens." << std::endl;
            }
            return;
        }

        // 3. Interpreting
        Interpreter interpreter;
        interpreter.interpret(statements);

    } catch (const std::exception& e) {
        std::cerr << "[Critical Error] " << e.what() << std::endl;
    }
}

void run_file(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    if (content.empty()) {
        std::cerr << "File is empty: " << path << std::endl;
        return;
    }

    run(content, path);
}

void run_prompt() {
    std::cout << "Xerith REPL 1.0 - Type a command and end with ';'\n";
    for (;;) {
        std::cout << "xerith> ";
        std::string line;
        if (!std::getline(std::cin, line)) break;
        if (line == "exit") break;
        run(line, "repl");
    }
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cout << "Usage: xerith [script]" << std::endl;
        return 64;
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        run_prompt();
    }
    return 0;
}
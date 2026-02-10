#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "utils/logging.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/ast_printer.h"

using namespace xerith;

void run(const std::string& source, const std::string& filename) {
    Lexer lexer(source, filename);
    std::vector<Token> tokens = lexer.scan_tokens();

    /*
    std::cout << "--- SCAN RESULTS ---\n";
    for (const auto& token : tokens) {
        std::cout << token.to_string() << "\n";
    }
    */

    // Phase 3: Parsing
    Parser parser(tokens);
    std::unique_ptr<Expr> expression = parser.parse();

    if (expression) {
        ASTPrinter printer;
        std::cout << "\n--- PARSE RESULT (AST) ---\n";
        std::cout << printer.print(expression.get()) << "\n";
    } else {
        std::cerr << "[ERROR] Could not parse the input source.\n";
    }
}

void run_file(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        exit(74);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    
    std::cout << "[INFO] Compiling: " << path << "\n";
    run(buffer.str(), path);
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cout << "Usage: xerith [script]" << std::endl;
        exit(64);
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        std::cout << "Xerith REPL - Enter expressions (Ctrl+C to exit)\n> ";
        std::string line;
        while (std::getline(std::cin, line)) {
            run(line, "repl");
            std::cout << "> ";
        }
    }

    return 0;
}
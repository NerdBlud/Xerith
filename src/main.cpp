#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "runtime/interpreter.h"

using namespace xerith;

void run(const std::string& source, const std::string& filename, Interpreter& interpreter) {
    Lexer lexer(source, filename);
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser(tokens);
    std::vector<std::unique_ptr<Stmt>> statements = parser.parse();

    interpreter.interpret(statements);
}

int main(int argc, char* argv[]) {
    Interpreter interpreter;
    if (argc > 1) {
        std::ifstream file(argv[1]);
        std::stringstream buffer;
        buffer << file.rdbuf();
        run(buffer.str(), argv[1], interpreter);
    } else {
        std::string line;
        while (std::cout << "> " && std::getline(std::cin, line)) {
            run(line, "repl", interpreter);
        }
    }
    return 0;
}
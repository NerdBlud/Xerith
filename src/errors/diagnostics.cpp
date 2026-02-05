#include "diagnostics.h"
#include <iostream>
#include <fstream>
#include <string>

namespace xerith {

void Diagnostics::report(const Error& err) {
    // 1. Print the human-readable header we built in error.cpp
    // This uses the format_full_error function from Step 4
    std::cout << format_full_error(err) << std::endl;

    // 2. Print the visual source context if the span is valid
    if (err.location.line > 0) {
        print_source_line(err.location);
    }
    std::cout << std::endl; // Extra spacing for readability
}

void Diagnostics::report_all(const std::vector<Error>& errors) {
    for (const auto& err : errors) {
        report(err);
    }
}

void Diagnostics::print_source_line(const Span& span) {
    std::ifstream file(span.filename);
    if (!file.is_open()) return;

    std::string line_text;
    // Skip lines until we reach the target line
    for (int i = 1; i <= span.line; ++i) {
        if (!std::getline(file, line_text)) break;
    }

    if (line_text.empty()) return;

    // Print the line number and the code
    std::cout << "  " << span.line << " | " << line_text << std::endl;

    // Print the caret pointer
    // We add spaces equal to (line number prefix + current column - 1)
    std::cout << "    | ";
    for (int i = 1; i < span.column; ++i) {
        std::cout << " ";
    }
    std::cout << "\033[1;31m^\033[0m" << std::endl; // Bold Red Caret
}

} // namespace xerith
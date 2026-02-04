#ifndef XERITH_SPAN_H
#define XERITH_SPAN_H

#include <string>
#include <iostream>

namespace xerith {

/**
 * @brief Represents a location within a source file.
 * Essential for providing "Easy Job" diagnostics to the user.
 */
struct Span {
    std::string filename;
    int line;
    int column;

    // default constructor for empty spans
    Span() : filename("unknown"), line(0), column(0) {}

    // detailed constructor
    Span(std::string file, int ln, int col) 
        : filename(std::move(file)), line(ln), column(col) {}

    // Helper to print the span in a standard format: "file:line:col"
    void print(std::ostream& os) const {
        os << filename << ":" << line << ":" << column;
    }
};

} 

#endif 
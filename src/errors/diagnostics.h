#ifndef XERITH_DIAGNOSTICS_H
#define XERITH_DIAGNOSTICS_H

#include "error.h"
#include <vector>

namespace xerith {

class Diagnostics {
public:
    /**
     * @brief Reports a single error to the console with source context.
     * Includes the file snippet and caret pointing to the column.
     */
    static void report(const Error& err);

    /**
     * @brief Reports a batch of errors.
     */
    static void report_all(const std::vector<Error>& errors);

private:
    // Helper to fetch and print the specific line from the source file
    static void print_source_line(const Span& span);
};

} // namespace xerith

#endif // XERITH_DIAGNOSTICS_H
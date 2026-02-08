#include "utils/logging.h"
#include "errors/diagnostics.h"

using namespace xerith;

int main() {
    Logger::info("Xerith Compiler Initialized...");

    Span testSpan("main.xrth", 5, 12);
    Error myError(
        ErrorType::Syntax, 
        Severity::Error, 
        testSpan, 
        "Expected a semicolon after the expression.", 
        "E102"
    );

    Diagnostics::report(myError);

    return 0;
}
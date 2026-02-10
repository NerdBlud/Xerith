#ifndef XERITH_PRECEDENCE_H
#define XERITH_PRECEDENCE_H

namespace xerith {

enum class Precedence {
    NONE,
    ASSIGNMENT,  // =
    OR,          // or
    AND,         // and
    EQUALITY,    // == !=
    COMPARISON,  // < > <= >=
    TERM,        // + -
    FACTOR,      // * /
    UNARY,       // ! -
    CALL,        // . ()
    PRIMARY
};

} // namespace xerith

#endif
# Xerith Implementation List

## Phase 1: Core Infrastructure & Utilities 

These are the foundation. You can't log errors or run anything without them.

* [`src/utils/span.h`](src/utils/span.h) – Defines the `Span` struct (line, column, filename).
* [`src/utils/logging.h`](src/utils/logging.h) / [`src/utils/logging.cpp`](src/utils/logging.cpp) – Basic logging to see what the compiler is doing.
* [`src/utils/arena.h`](src/utils/arena.h) / [`src/utils/arena.cpp`](src/utils/arena.cpp) – Memory allocator to keep Xerith fast.
* [`src/errors/error.h`](src/errors/error.h) / [`src/errors/error.cpp`](src/errors/error.cpp) – Core error definitions.
* [`src/errors/diagnostics.h`](src/errors/diagnostics.h) / [`src/errors/diagnostics.cpp`](src/errors/diagnostics.cpp) – Prints snippets of code where errors happen.
* [`CMakeLists.txt`](CMakeLists.txt) – Sets up the build system to find LLVM and include these folders.

---

## Phase 2: Lexical Analysis

Turning raw text into tokens the compiler can understand.

* [`src/lexer/token.h`](src/lexer/token.h) / [`src/lexer/token.cpp`](src/lexer/token.cpp) – Defines keywords (`fn`, `let`, `if`) and literal types.
* [`src/lexer/lexer.h`](src/lexer/lexer.h) / [`src/lexer/lexer.cpp`](src/lexer/lexer.cpp) – Scanner that converts text into tokens.
* [`src/main.cpp`](src/main.cpp) – Reads a `.xrth` file and prints the tokens (initial version).

---

## Phase 3: Parser & Abstract Syntax Tree (AST)

Organizing tokens into a structure that represents the code.

* [`src/parser/precedence.h`](src/parser/precedence.h) – Defines operator precedence (`*` stronger than `+`).
* [`src/parser/ast.h`](src/parser/ast.h) / [`src/parser/ast.cpp`](src/parser/ast.cpp) – Classes for all language constructs (e.g., `BinaryExpr`, `VarDecl`).
* [`src/parser/parser.h`](src/parser/parser.h) / [`src/parser/parser.cpp`](src/parser/parser.cpp) – Converts tokens into an AST.

---

## Phase 4: Semantic Analysis

Checking that the code actually makes sense.

* [`src/sema/symbols.h`](src/sema/symbols.h) / [`src/sema/symbols.cpp`](src/sema/symbols.cpp) – Symbol table to track variables and scopes.
* [`src/sema/resolver.h`](src/sema/resolver.h) / [`src/sema/resolver.cpp`](src/sema/resolver.cpp) – Makes sure variables are declared before they’re used.

---

## Phase 5: Runtime & Virtual Machine

Where Xerith starts running programs.

* [`src/runtime/value.h`](src/runtime/value.h) / [`src/runtime/value.cpp`](src/runtime/value.cpp) – Defines what a `Value` is (Integer, String, Object, etc.).
* [`src/runtime/environment.h`](src/runtime/environment.h) / [`src/runtime/environment.cpp`](src/runtime/environment.cpp) – Keeps track of memory and program state.
* [`src/vm/bytecode.h`](src/vm/bytecode.h) / [`src/vm/bytecode.cpp`](src/vm/bytecode.cpp) – Defines low-level instructions for the VM.
* [`src/vm/disasm.h`](src/vm/disasm.h) – Lets you inspect raw bytecode for debugging.
* [`src/vm/vm.h`](src/vm/vm.h) / [`src/vm/vm.cpp`](src/vm/vm.cpp) – The engine that executes bytecode.
* [`src/runtime/builtins.h`](src/runtime/builtins.h) / [`src/runtime/builtins.cpp`](src/runtime/builtins.cpp) – Native functions like `print`.
* [`src/runtime/interpreter.h`](src/runtime/interpreter.h) / [`src/runtime/interpreter.cpp`](src/runtime/interpreter.cpp) – Runs the AST directly before the VM is ready.

---

## Phase 6: Standard Library & Tools

Finishing touches for users.

* [`std/io.xrth`](std/io.xrth), [`std/math.xrth`](std/math.xrth), [`std/strings.xrth`](std/strings.xrth) – Core library functions in Xerith.
* [`tools/repl/`](tools/repl/) – Interactive Read-Eval-Print-Loop.
* [`tools/formatter/`](tools/formatter/) – Auto-formatter for `.xrth` code.
* [`docs/`](docs/) – Language reference (`language-spec.md`, `syntax.md`) for users.

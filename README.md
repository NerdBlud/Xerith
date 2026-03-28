# Xerith

Xerith is a systems-oriented programming language designed for granular control over the compilation pipeline. This implementation serves as a technical exploration of recursive descent parsing, Abstract Syntax Tree (AST) architecture, and tree-walk evaluation.

## Architecture

The project is structured as a standard compiler front-end:

* **Lexer:** A hand-written scanner for deterministic tokenization.
* **Parser:** A recursive descent implementation with operator precedence handling.
* **AST Implementation:** A strongly-typed tree structure for intermediate representation.
* **Interpreter:** A visitor-pattern based evaluator that decouples execution logic from node definitions.

## Key Design Principles

* **Zero Dependencies:** The core implementation avoids external libraries to ensure a minimal, build-stable binary.
* **AST Integrity:** Prioritizing strict node definitions to facilitate easier semantic analysis in future iterations.
* **Transparent Tooling:** Integrated CLI for debugging token streams and AST dumps.

## Implementation Roadmap

- [x] Lexical analysis and Token stream generation
- [x] Abstract Syntax Tree (AST) core nodes
- [x] Visitor Pattern infrastructure for evaluation
- [ ] Environment management and Lexical Scoping
- [ ] Control flow primitives (If/While)
- [ ] Function declarations and stack frame handling
- [ ] Bytecode IR and Virtual Machine (Target)

## Build System

Xerith uses **CMake** for build orchestration and is developed primarily in a **C++17/20** environment.

```bash
mkdir build && cd build
cmake ..
make
./xerith path/to/script.xrtx
```

## Trademark & Licensing

The name **“Xerith”** is a registered trademark of NerdBlud. 
The source code is licensed under **AGPL v3**. You are free to fork and modify the implementation, but redistribution under the "Xerith" name or confusingly similar branding requires explicit permission.

---
*Developed by nerdblud*

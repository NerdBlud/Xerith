cmake_minimum_required(VERSION 3.10)
project(Xerith VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(SOURCES
    src/main.cpp

    src/utils/logging.cpp
    src/utils/arena.cpp
    src/errors/error.cpp
    src/errors/diagnostics.cpp

    src/lexer/token.cpp
    src/lexer/lexer.cpp

    src/parser/ast.cpp
    src/parser/parser.cpp
    src/parser/ast_printer.cpp

    src/runtime/environment.cpp
    src/runtime/interpreter.cpp
)

add_executable(xerith ${SOURCES})

llvm_map_components_to_libnames(llvm_libs core support native)
target_link_libraries(xerith ${llvm_libs})

target_include_directories(xerith PRIVATE src)
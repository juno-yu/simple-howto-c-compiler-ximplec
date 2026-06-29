cmake_minimum_required(VERSION 3.14)
project(simplecc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fetch Catch2 for testing
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

# Main compiler library
add_library(compiler_lib
    src/lexer.cpp
    src/ast.cpp
    src/parser.cpp
    src/codegen.cpp
    src/preprocessor.cpp
    src/semantic.cpp
    src/compiler.cpp
)
target_include_directories(compiler_lib PUBLIC src)

# Main executable
add_executable(simplecc src/main.cpp)
target_link_libraries(simplecc compiler_lib)

# Tests
enable_testing()
add_subdirectory(0001-tokenizer/tests)
add_subdirectory(0002-ast/tests)
add_subdirectory(0003-parser/tests)
add_subdirectory(0004-codegen/tests)
add_subdirectory(0005-integration/tests)
add_subdirectory(0076-register-auto/tests)
add_subdirectory(0086-nested-functions/tests)
add_subdirectory(0043-float-double-bit-pattern/tests)
add_subdirectory(0043-float-double-sse/tests)

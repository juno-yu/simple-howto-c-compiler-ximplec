# Simple C Compiler in C++

A step-by-step implementation of a simple C compiler, teaching compiler construction through incremental lessons.

## Overview

This project builds a compiler for a subset of C, progressing from basic tokenization to code generation. Each lesson is self-contained with its own source code, tests, and documentation.

## Learning Path

```
Source Code → Tokenizer → Parser → AST → Code Generator → Executable
     │            │          │        │           │
     ▼            ▼          ▼        ▼           ▼
  0001-       0002-       0003-    0004-       0005-
  Tokenizer   Parser      AST      Codegen     Integration
```

## Project Structure

```
├── src/                          # Combined, evolving source
│   ├── token.h                   # Token types and structures
│   ├── lexer.h/cpp               # Lexer implementation
│   ├── ast.h/cpp                 # AST node definitions
│   ├── parser.h/cpp              # Recursive descent parser
│   ├── codegen.h/cpp             # x86-64 code generation
│   ├── compiler.h/cpp            # Compiler orchestrator
│   └── main.cpp                  # CLI entry point
├── 0001-tokenizer/               # Lesson 1: Lexical analysis
│   ├── README.md                 # Detailed explanation
│   ├── src/                      # Lesson-specific source
│   └── tests/                    # Unit tests
├── 0002-ast/                     # Lesson 2: AST definitions
├── ...                           # Lessons 3-40 (see Progress Tracker)
├── 0040-optimizations/           # Lesson 40: Optimization passes
├── CMakeLists.txt                # Build configuration
└── README.md                     # This file
```

## Supported C Subset

| Feature | Status |
|---------|--------|
| Integer literals | ✅ Implemented |
| Char literals | ✅ Implemented |
| Variables (int, char, bool) | ✅ Implemented |
| Arithmetic operators (+, -, *, /, %) | ✅ Implemented |
| Comparison operators (==, !=, <, >, <=, >=) | ✅ Implemented |
| Logical operators (&&, \|\|, !) | ✅ Implemented |
| Bitwise operators (&, \|, ^, ~, <<, >>) | ✅ Implemented |
| Assignment operators (=) | ✅ Implemented |
| Compound assignment (+=, -=, *=, /=) | ✅ Implemented |
| Increment/Decrement (++, --) | ✅ Implemented |
| Ternary operator (? :) | ✅ Implemented |
| Comma operator (,) | ✅ Implemented |
| Control flow (if/else, while, for, do-while) | ✅ Implemented |
| Functions with parameters | ✅ Implemented |
| Local variables | ✅ Implemented |
| Recursive functions | ✅ Implemented (fibonacci works) |
| String literals | ⚠️ Parsed, codegen stub |
| Float types | ❌ Not implemented |
| Arrays | ❌ Not implemented |
| Pointers | ❌ Not implemented |
| Structs | ❌ Not implemented |
| sizeof | ❌ Token exists, parser pending |
| Preprocessor | ❌ Not implemented |

## Building

```bash
mkdir build && cd build
cmake ..
make
```

## Running Tests

```bash
cd build
ctest --output-on-failure
```

## Running the Compiler

```bash
# Compile a C file to assembly
./build/simplecc -S test.c

# Compile and assemble
./build/simplecc test.c
gcc -no-pie test.s -o test
./test; echo $?
```

## Target Architecture

- x86-64 Linux (System V ABI)
- Generates GAS syntax assembly
- Assembled with GNU as, linked with GNU ld/gcc

## Example

Input (`test.c`):
```c
int add(int a, int b) { return a + b; }
int fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}
int main() {
    int sum = add(10, 20);
    int f10 = fib(10);
    return sum + f10;
}
```

Output: Exit code 85 (30 + 55)

## Lesson Progress

### Core Lessons (0001-0005) - ✅ Complete

| Lesson | Topic | Tests | Status |
|--------|-------|-------|--------|
| 0001 | Tokenizer (Lexer) | 26 | ✅ Complete |
| 0002 | AST Definitions | 10 | ✅ Complete |
| 0003 | Parser (Recursive Descent) | 20 | ✅ Complete |
| 0004 | Code Generator (x86-64) | 11 | ✅ Complete |
| 0005 | Integration (CLI, Pipeline) | 11 | ✅ Complete |

### Quick Wins (0006-0012) - ✅ Complete

| Lesson | Topic | Status |
|--------|-------|--------|
| 0006 | Compound Assignment | ✅ Complete |
| 0007 | Ternary Operator | ✅ Complete |
| 0008 | Do-While Loops | ✅ Complete |
| 0009 | Comma Operator | ✅ Complete |
| 0010 | Bool Type | ✅ Complete |
| 0011 | Forward Declarations | 📋 Planned |
| 0012 | const Qualifier | 📋 Planned |

### Feature Lessons (0013-0075) - 📋 Planned

| Lesson | Topic | Complexity | Status |
|--------|-------|------------|--------|
| 0013 | Type System | Hard | 📋 Planned |
| 0014 | sizeof | Easy | 📋 Planned |
| 0015 | Signed/Unsigned | Medium | 📋 Planned |
| 0016 | Type Casts | Medium | 📋 Planned |
| 0017 | Type Promotions | Medium | 📋 Planned |
| 0018 | Type-Aware Codegen | Hard | 📋 Planned |
| 0019 | String Literal Codegen | Easy | 📋 Planned |
| 0020 | Global Variables | Medium | 📋 Planned |
| 0021 | Extern Declarations | Easy | 📋 Planned |
| 0022 | Struct Declarations | Hard | 📋 Planned |
| 0023 | Struct Access | Medium | 📋 Planned |
| 0024 | Pointer Types | Hard | 📋 Planned |
| 0025 | Array Types | Hard | 📋 Planned |
| 0026 | Pointer Arithmetic | Medium | 📋 Planned |
| 0027-0075 | More features... | Various | 📋 Planned |
|--------|-------|------------|--------|
| 0016 | Ternary Operator | Easy | 📋 Planned |
| 0017 | Compound Assignment | Easy | 📋 Planned |
| 0018 | sizeof Operator | Easy | 📋 Planned |
| 0019 | Signed/Unsigned and Integer Sizes | Medium | 📋 Planned |
| 0020 | Float and Double (SSE) | Hard | 📋 Planned |
| 0021 | Unions | Medium | 📋 Planned |
| 0022 | Bitfields | Medium | 📋 Planned |
| 0023 | Comma Operator | Easy | 📋 Planned |
| 0024 | Explicit Type Casts | Medium | 📋 Planned |
| 0025 | Type Promotions | Medium | 📋 Planned |
| 0026 | _Bool / bool Type | Easy | 📋 Planned |

### Advanced C Lessons (0027-0030) - 📋 Planned

| Lesson | Topic | Complexity | Status |
|--------|-------|------------|--------|
| 0027 | Designated Initializers (C99) | Medium | 📋 Planned |
| 0028 | Compound Literals (C99) | Medium | 📋 Planned |
| 0029 | _Static_assert (C11) | Easy | 📋 Planned |
| 0030 | _Generic Selection (C11) | Hard | 📋 Planned |

### Preprocessor Lessons (0031-0032) - 📋 Planned

| Lesson | Topic | Complexity | Status |
|--------|-------|------------|--------|
| 0031 | Preprocessor Macros | Medium | 📋 Planned |
| 0032 | Conditional Compilation | Medium | 📋 Planned |

### Linkage and Storage Lessons (0033-0038) - 📋 Planned

| Lesson | Topic | Complexity | Status |
|--------|-------|------------|--------|
| 0033 | Forward Declarations | Easy | 📋 Planned |
| 0034 | Static Linkage | Easy | 📋 Planned |
| 0035 | Variadic Functions | Hard | 📋 Planned |
| 0036 | const Qualifier | Easy | 📋 Planned |
| 0037 | Static Local Variables | Medium | 📋 Planned |
| 0038 | volatile Qualifier | Easy | 📋 Planned |

### System Lessons (0039-0040) - 📋 Planned

| Lesson | Topic | Complexity | Status |
|--------|-------|------------|--------|
| 0039 | Basic Standard Library | Hard | 📋 Planned |
| 0040 | Compiler Optimizations | Hard | 📋 Planned |

## Recommended Implementation Order

### Phase 1: Quick Wins (leverage existing code)
1. **0017** - Compound Assignment (tokens exist, parser change only)
2. **0023** - Comma Operator (trivial)
3. **0016** - Ternary Operator (moderate)
4. **0026** - _Bool type (simple)
5. **0033** - Forward Declarations (parser change)
6. **0036** - const qualifier (type system)

### Phase 2: Type System Expansion
7. **0018** - sizeof + casts
8. **0019** - unsigned/long types
9. **0021** - Unions
10. **0037** - Static local variables
11. **0029** - _Static_assert

### Phase 3: Data Structures
12. **0006** - String Literals (complete codegen)
13. **0007** - Arrays
14. **0008** - Structs
15. **0009** - Pointers

### Phase 4: Control Flow
16. **0011** - Do-While + break/continue
17. **0012** - Switch statements
18. **0013** - Goto and labels

### Phase 5: Expressions
19. **0024** - Explicit type casts
20. **0025** - Type promotions
21. **0027** - Designated initializers
22. **0028** - Compound literals

### Phase 6: Advanced
23. **0020** - Float/Double (SSE codegen)
24. **0022** - Bitfields
25. **0031-0032** - Preprocessor
26. **0034-0035** - Static/Variadic

### Phase 7: System Integration
27. **0014** - Global variables
28. **0015** - Multi-file compilation
29. **0039** - Standard library
30. **0040** - Optimizations

## Git History

```
Initial commit:
  - 0001-tokenizer: 26 tests passing
  - 0002-ast: 10 tests passing
  - 0003-parser: 20 tests passing
  - 0004-codegen: 11 tests passing
  - 0005-integration: 11 tests passing
  - 40 lesson READMEs with mermaid diagrams
  - All lessons documented with checklists
```

## References

- [Compilers: Principles, Techniques, and Tools](https://dragonbook.stanford.edu/)
- [chibicc - Small C Compiler](https://github.com/rui314/chibicc)
- [Writing a C Compiler](https://norasandler.com/2017/11/29/Write-a-Compiler.html)
- [x86-64 SysV ABI](https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf)
- [GCC Inline Assembly](https://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html)

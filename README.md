# Simple C Compiler in C++

A step-by-step implementation of a simple C compiler in C++, targeting x86-64 Linux with GAS syntax assembly. Goal: self-hosting.

## Overview

This project builds a compiler for a substantial subset of C, progressing from basic tokenization to code generation. Each lesson is self-contained with its own source code, tests, and documentation.

## Learning Path

```
Source Code → Tokenizer → Parser → AST → Code Generator → Executable
     │            │          │        │           │
     ▼            ▼          ▼        ▼           ▼
  0001-       0002-       0003-    0004-       0005-
  Tokenizer   AST         Parser   Codegen     Integration
```

## Project Structure

```
├── src/                          # Combined, evolving source
│   ├── token.h                   # Token types and structures
│   ├── lexer.h/cpp               # Lexer implementation
│   ├── ast.h/cpp                 # AST node definitions (30+ node types)
│   ├── parser.h/cpp              # Recursive descent parser (~1000 lines)
│   ├── codegen.h/cpp             # x86-64 code generator (~1100 lines)
│   ├── compiler.h/cpp            # Compiler orchestrator
│   └── main.cpp                  # CLI entry point
├── 0001-tokenizer/               # Lesson 1: Lexical analysis
├── ...                           # Lessons 2-75
├── 0075-bootstrap/               # Lesson 75: Self-hosting bootstrap
├── 1000-c11-static-assert/       # C11 standard lessons
├── ...                           # C11/C17/C23 lessons
├── 3014-c23-predef-macros/       # C23 predefined macros
├── CMakeLists.txt                # Build configuration
└── README.md                     # This file
```

## Supported C Subset

| Feature | Status |
|---------|--------|
| **Literals** | |
| Integer literals (decimal, hex, binary, octal) | ✅ Implemented |
| Char literals | ✅ Implemented |
| String literals (.rodata, .asciz) | ✅ Implemented |
| Float literals | ✅ Parsed |
| **Types** | |
| int, char, void, bool | ✅ Implemented |
| long, short, signed, unsigned | ✅ Parsed |
| Pointer types (int*, char**) | ✅ Implemented |
| Array types (int arr[10]) | ✅ Implemented |
| struct declarations & member access | ✅ Implemented |
| enum declarations & constant folding | ✅ Implemented |
| typedef declarations | ✅ Implemented |
| union declarations | 📋 Parsed |
| **Operators** | |
| Arithmetic (+, -, *, /, %) | ✅ Implemented |
| Comparison (==, !=, <, >, <=, >=) | ✅ Implemented |
| Logical (&&, \|\|, !) | ✅ Implemented |
| Bitwise (&, \|, ^, ~, <<, >>) | ✅ Implemented |
| Assignment (=) | ✅ Implemented |
| Compound assignment (+=, -=, *=, /=) | ✅ Implemented |
| Increment/Decrement (++, --) | ✅ Implemented |
| Ternary (? :) | ✅ Implemented |
| Comma operator | ✅ Implemented |
| sizeof (type and expression) | ✅ Implemented |
| Type casts (int)expr | ✅ Implemented |
| Address-of (&) and dereference (*) | ✅ Implemented |
| Member access (.) and arrow (->) | ✅ Implemented |
| Array indexing (arr[i]) | ✅ Implemented |
| **Control Flow** | |
| if/else | ✅ Implemented |
| while loops | ✅ Implemented |
| do-while loops | ✅ Implemented |
| for loops | ✅ Implemented |
| switch/case/default | ✅ Implemented |
| break/continue | ✅ Implemented |
| goto/labels | ✅ Implemented |
| **Functions** | |
| Function declarations & definitions | ✅ Implemented |
| Forward declarations (prototypes) | ✅ Implemented |
| System V ABI (rdi, rsi, rdx, rcx, r8, r9) | ✅ Implemented |
| Recursive functions | ✅ Implemented |
| Function pointers | 📋 Parsed |
| **Variables** | |
| Local variables | ✅ Implemented |
| Global variables (.data, RIP-relative) | ✅ Implemented |
| extern declarations | ✅ Implemented |
| const qualifier | ✅ Implemented |
| **Other** | |
| Multi-file compilation | ❌ Not implemented |
| Preprocessor (#include, #define) | ❌ Not implemented |
| Standard library stubs | ❌ Not implemented |

## Building

```bash
mkdir build && cd build
cmake ..
make -j4
```

## Running Tests

```bash
cd build
ctest --output-on-failure
```

## Running the Compiler

```bash
# Compile a C file to assembly
./build/simplecc -S input.c

# Compile and run
./build/simplecc -S input.c -o output.s
gcc -o output output.s
./output; echo $?
```

## Target Architecture

- x86-64 Linux (System V ABI)
- Generates GAS syntax assembly
- Assembled with GNU as, linked with GNU ld/gcc

## Example

Input:
```c
struct Point { int x; int y; };

int main() {
    struct Point p;
    p.x = 10;
    p.y = 20;
    return p.x + p.y;
}
```

Output: Exit code 30 (10 + 20)

## Verified Working Examples

```c
// Pointer dereference → 42
int x = 42; int *p = &x; return *p;

// Array indexing → 20
int arr[3]; arr[1] = 20; return arr[1];

// Switch/case → 20
switch (x) { case 1: return 10; case 2: return 20; }

// Enum constants → 1
enum Color { RED=0, GREEN=1 }; return GREEN;

// Typedef → 42
typedef int integer; integer x = 42; return x;

// Struct member access → 30
struct Point { int x; int y; };
struct Point p; p.x=10; p.y=20; return p.x+p.y;
```

## Lesson Progress

### Core Lessons (0001-0005) — ✅ Complete

| Lesson | Topic | Tests | Status |
|--------|-------|-------|--------|
| 0001 | Tokenizer (Lexer) | 26 | ✅ Complete |
| 0002 | AST Definitions | 10 | ✅ Complete |
| 0003 | Parser (Recursive Descent) | 20 | ✅ Complete |
| 0004 | Code Generator (x86-64) | 11 | ✅ Complete |
| 0005 | Integration (CLI, Pipeline) | 11 | ✅ Complete |

### Quick Wins (0006-0012) — ✅ Complete

| Lesson | Topic | Status |
|--------|-------|--------|
| 0006 | Compound Assignment | ✅ Implemented |
| 0007 | Ternary Operator | ✅ Implemented |
| 0008 | Do-While Loops | ✅ Implemented |
| 0009 | Comma Operator | ✅ Implemented |
| 0010 | Bool Type | ✅ Implemented |
| 0011 | Forward Declarations | ✅ Implemented |
| 0012 | const Qualifier | ✅ Implemented |

### Type System (0013-0018) — ✅ Complete

| Lesson | Topic | Status |
|--------|-------|--------|
| 0013 | Type System | ✅ Implemented |
| 0014 | sizeof | ✅ Implemented |
| 0015 | Signed/Unsigned | ✅ Implemented |
| 0016 | Type Casts | ✅ Implemented |
| 0017 | Type Promotions | ✅ Implemented |
| 0018 | Type-Aware Codegen | ✅ Implemented |

### Strings & Globals (0019-0021) — ✅ Complete

| Lesson | Topic | Status |
|--------|-------|--------|
| 0019 | String Literal Codegen | ✅ Implemented |
| 0020 | Global Variables | ✅ Implemented |
| 0021 | Extern Declarations | ✅ Implemented |

### Structs & Pointers (0022-0025) — ✅ Complete

| Lesson | Topic | Status |
|--------|-------|--------|
| 0022 | Struct Declarations | ✅ Implemented |
| 0023 | Struct Access | ✅ Implemented |
| 0024 | Pointer Types | ✅ Implemented |
| 0025 | Array Types | ✅ Implemented |

### Advanced Features (0026-0032) — ✅ Complete

| Lesson | Topic | Status |
|--------|-------|--------|
| 0026 | Pointer Arithmetic | ✅ Implemented |
| 0027 | Unions | ✅ Parsed |
| 0028 | Enums | ✅ Implemented |
| 0029 | Typedef | ✅ Implemented |
| 0030 | Switch/Case | ✅ Implemented |
| 0031 | Goto/Labels | ✅ Implemented |
| 0032 | Break/Continue | ✅ Implemented |

### Preprocessor (0033-0035) — 📋 Planned

| Lesson | Topic | Status |
|--------|-------|--------|
| 0033 | Preprocessor Macros | 📋 Planned |
| 0034 | Conditional Compilation | 📋 Planned |
| 0035 | Include Directive | 📋 Planned |

### Advanced C (0036-0045) — 📋 Planned

| Lesson | Topic | Status |
|--------|-------|--------|
| 0036 | Function Pointers | 📋 Planned |
| 0037 | Void Pointers | 📋 Planned |
| 0038 | Designated Init | 📋 Planned |
| 0039 | Compound Literals | 📋 Planned |
| 0040 | Bitfields | 📋 Planned |
| 0041 | Multi-Dim Arrays | 📋 Planned |
| 0042 | Array-Pointer Decay | 📋 Planned |
| 0043 | Float/Double | 📋 Planned |
| 0044 | Static Assert | 📋 Planned |
| 0045 | Generic Selection | 📋 Planned |

### System & Optimization (0046-0075) — 📋 Planned

| Lesson | Topic | Status |
|--------|-------|--------|
| 0046-0051 | Variadic, Inline ASM, etc. | 📋 Planned |
| 0052-0055 | Standard Library | 📋 Planned |
| 0056-0065 | System Programming | 📋 Planned |
| 0066-0070 | Optimizations & Debug | 📋 Planned |
| 0071-0075 | Self-Hosting | 📋 Planned |

### C11 Standard (1000-1015) — 📋 Planned

| Lesson | Topic | Status |
|--------|-------|--------|
| 1000-1015 | _Static_assert, _Generic, _Atomic, etc. | 📋 Planned |

### C17 Standard (2000-2005) — 📋 Planned

| Lesson | Topic | Status |
|--------|-------|--------|
| 2000-2005 | stdbool, stdint, typeof, etc. | 📋 Planned |

### C23 Standard (3000-3014) — 📋 Planned

| Lesson | Topic | Status |
|--------|-------|--------|
| 3000-3014 | bool literal, auto, nullptr, attributes, etc. | 📋 Planned |

## Test Results

```
5/5 Test #1: tokenizer_tests ..................   Passed
5/5 Test #2: ast_tests ........................   Passed
5/5 Test #3: parser_tests .....................   Passed
5/5 Test #4: codegen_tests ....................   Passed
5/5 Test #5: integration_tests ................   Passed
100% tests passed, 0 tests failed
```

## References

- [Compilers: Principles, Techniques, and Tools](https://dragonbook.stanford.edu/)
- [chibicc - Small C Compiler](https://github.com/rui314/chibicc)
- [Writing a C Compiler](https://norasandler.com/2017/11/29/Write-a-Compiler.html)
- [x86-64 SysV ABI](https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf)

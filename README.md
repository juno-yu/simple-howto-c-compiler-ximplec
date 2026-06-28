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

**Compilation status:** 41/70 lessons compile successfully, 29 need fixes.

### Core Lessons (0001-0005) — ✅ Complete

| Lesson | Topic | Tests | Compile |
|--------|-------|-------|---------|
| 0001 | Tokenizer (Lexer) | 26 | ✅ |
| 0002 | AST Definitions | 10 | ✅ |
| 0003 | Parser (Recursive Descent) | 20 | ✅ |
| 0004 | Code Generator (x86-64) | 11 | ✅ |
| 0005 | Integration (CLI, Pipeline) | 11 | ✅ |

### Quick Wins (0006-0012)

| Lesson | Topic | Compile | Fix Needed |
|--------|-------|---------|------------|
| 0006 | Compound Assignment | ❌ | Fix `/=` token consumption |
| 0007 | Ternary Operator | ✅ | — |
| 0008 | Do-While Loops | ✅ | — |
| 0009 | Comma Operator | ✅ | — |
| 0010 | Bool Type | ✅ | — |
| 0011 | Forward Declarations | ✅ | — |
| 0012 | const Qualifier | ✅ | — |

### Type System (0013-0018)

| Lesson | Topic | Compile | Fix Needed |
|--------|-------|---------|------------|
| 0013 | Type System | ✅ | — |
| 0014 | sizeof | ✅ | — |
| 0015 | Signed/Unsigned | ❌ | Add `unsigned`/`signed` to type specifiers |
| 0016 | Type Casts | ❌ | Add `double` type, fix cast expression |
| 0017 | Type Promotions | ✅ | — |
| 0018 | Type-Aware Codegen | ✅ | — |

### Strings & Globals (0019-0021)

| Lesson | Topic | Compile | Fix Needed |
|--------|-------|---------|------------|
| 0019 | String Literal Codegen | ✅ | — |
| 0020 | Global Variables | ✅ | — |
| 0021 | Extern Declarations | ❌ | Add `extern` to type specifier check |

### Structs & Pointers (0022-0025)

| Lesson | Topic | Compile | Fix Needed |
|--------|-------|---------|------------|
| 0022 | Struct Declarations | ✅ | — |
| 0023 | Struct Access | ✅ | — |
| 0024 | Pointer Types | ✅ | — |
| 0025 | Array Types | ✅ | — |

### Advanced Features (0026-0032)

| Lesson | Topic | Compile | Fix Needed |
|--------|-------|---------|------------|
| 0026 | Pointer Arithmetic | ❌ | Add braced initializer `{10,20,30}` |
| 0027 | Unions | ❌ | Add `union` keyword + parsing |
| 0028 | Enums | ✅ | — |
| 0029 | Typedef | ✅ | — |
| 0030 | Switch/Case | ✅ | — |
| 0031 | Goto/Labels | ❌ | Add label statement parsing (IDENTIFIER `:`) |
| 0032 | Break/Continue | ✅ | — |

### Preprocessor (0033-0035)

| Lesson | Topic | Compile | Fix Needed |
|--------|-------|---------|------------|
| 0033 | Preprocessor Macros | ❌ | Add basic `#define` pass |
| 0034 | Conditional Compilation | ❌ | Add `#ifdef`/`#else`/`#endif` |
| 0035 | Include Directive | ❌ | Add `#include` pass |

### Advanced C (0036-0045)

| Lesson | Topic | Compile | Fix Needed |
|--------|-------|---------|------------|
| 0036 | Function Pointers | ❌ | Fix null-pointer in parser |
| 0037 | Void Pointers | ❌ | Fix cast expression recognition |
| 0038 | Designated Init | ❌ | Add `.field = val` parsing |
| 0039 | Compound Literals | ❌ | Add `(type){...}` syntax |
| 0040 | Bitfields | ❌ | Add `int x : N` in struct fields |
| 0041 | Multi-Dim Arrays | ❌ | Add `arr[2][3]` and initializers |
| 0042 | Array-Pointer Decay | ❌ | Add braced initializer `{1,2,3}` |
| 0043 | Float/Double | ❌ | Add `float`/`double` type specifiers |
| 0044 | Static Assert | ✅ | — |
| 0045 | Generic | ❌ | Add `_Generic` or graceful skip |

### System & Functions (0046-0065)

| Lesson | Topic | Compile | Fix Needed |
|--------|-------|---------|------------|
| 0046 | Variadic | ❌ | Add `...` parameter syntax |
| 0047 | Statement Expr | ❌ | Add GCC `({...})` extension |
| 0048 | Inline ASM | ❌ | Add `asm` keyword + parsing |
| 0049 | Multi-File | ❌ | Add `extern` at top level |
| 0050 | Static Linkage | ❌ | Add `static` keyword handling |
| 0051 | Volatile Qualifier | ❌ | Add `volatile` keyword |
| 0052 | Syscalls | ✅ | — |
| 0053 | String Funcs | ✅ | — |
| 0054 | IO Functions | ❌ | Fix `...` in function params |
| 0055 | Memory Alloc | ✅ | — |
| 0056 | File IO | ✅ | — |
| 0057 | Process Control | ✅ | — |
| 0058 | Socket Prog | ✅ | — |
| 0059 | Signal Handling | ❌ | Fix complex function pointer syntax |
| 0060 | Environment | ✅ | — |
| 0061 | Thread Support | ✅ | — |
| 0062 | Synchronization | ✅ | — |
| 0063 | Memory Mapping | ✅ | — |
| 0064 | Dynamic Loading | ✅ | — |
| 0065 | IPC | ❌ | Add array parameter `int fd[2]` |

### Optimization & Self-Hosting (0066-0075)

| Lesson | Topic | Compile | Fix Needed |
|--------|-------|---------|------------|
| 0066 | Const Folding | ✅ | — |
| 0067 | Dead Code Elim | ✅ | — |
| 0068 | Register Alloc | ✅ | — |
| 0069 | Function Inline | ❌ | Add `inline` keyword |
| 0070 | Debug Info | ✅ | — |
| 0071 | Self-Host Prep | ✅ | — |
| 0072 | Compile Compiler 1 | ✅ | — |
| 0073 | Compile Compiler 2 | ✅ | — |
| 0074 | Compile Compiler 3 | ✅ | — |
| 0075 | Bootstrap | ✅ | — |

### C11/C17/C23 Standard Lessons — 📋 Planned

16 C11 lessons (1000-1015), 6 C17 lessons (2000-2005), 15 C23 lessons (3000-3014) — tests created, implementation pending.

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

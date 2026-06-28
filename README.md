# Simple C Compiler in C++

A step-by-step implementation of a simple C compiler in C++, targeting x86-64 Linux with GAS syntax assembly. Goal: self-hosting.

## Overview

This project builds a compiler for a substantial subset of C, progressing from basic tokenization to code generation. Each lesson is self-contained with its own source code, tests, and documentation.

## Compilation Pipeline

```
┌─────────────────────────────────────────────────────────────────────┐
│                     COMPLETE COMPILATION PIPELINE                   │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐         │
│  │  .c     │    │Expanded │    │ Tokens  │    │   AST   │         │
│  │ Source  │───▶│   .c    │───▶│ Lexer   │───▶│ Parser  │         │
│  │ Code    │    │         │    │         │    │         │         │
│  └─────────┘    └─────────┘    └─────────┘    └────────┬────────┘  │
│       │              │              │                   │           │
│       ▼              ▼              ▼                   ▼           │
│  ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────────────┐  │
│  │Preproc  │    │  Preproc│    │ Lexer   │    │  Semantic       │  │
│  │#define  │    │ #ifdef  │    │ Token   │    │  Analysis       │  │
│  │#include │    │ #else   │    │ Stream  │    │  Type Check     │  │
│  │#ifdef   │    │ #endif  │    │         │    │  Symbol Table   │  │
│  └─────────┘    └─────────┘    └─────────┘    └────────┬────────┘  │
│                                                         │           │
│  ════════════════════ THIS PROJECT COVERS ════════════════════════  │
│                                                         │           │
│                                                         ▼           │
│                                                    ┌─────────┐     │
│                                                    │   .s    │     │
│                                                    │  GAS    │     │
│                                                    │Assembly │     │
│                                                    └────────┬┘     │
│                                                             │      │
│  ┌─────────────────────────────────────────────────────────┘       │
│  │                                                                 │
│  │  ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐     │
│  │  │   .s    │    │   .o    │    │  libs   │    │   ELF   │     │
│  │  │  GAS    │───▶│   ELF   │───▶│  libc   │───▶│  a.out  │     │
│  │  │  Text   │    │  Object │    │         │    │  Exec   │     │
│  │  └─────────┘    └─────────┘    └─────────┘    └─────────┘     │
│  │       │              │              │              │            │
│  │       ▼              ▼              ▼              ▼            │
│  │   GNU as           GNU ld/gcc    System Libs     Linux Kernel  │
│  │   Assembler        Linker        Standard C      ELF Loader    │
│  │                                                                 │
│  │  ═══════════════ EXTERNAL TOOLCHAIN (not this project) ════════│
│  └─────────────────────────────────────────────────────────────────┘
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

### Pipeline Stages

| Stage | Input | Output | Tool | This Project |
|-------|-------|--------|------|--------------|
| **Preprocessing** | `.c` + headers | Expanded `.c` | `cpp` / `gcc -E` | ✅ `src/preprocessor.cpp` |
| **Lexical Analysis** | `.c` source | Token stream | Lexer | ✅ `src/lexer.cpp` |
| **Parsing** | Token stream | AST | Parser | ✅ `src/parser.cpp` |
| **Semantic Analysis** | AST | Validated AST | Type checker | ✅ `src/semantic.cpp` |
| **Code Generation** | AST | Assembly (`.s`) | Codegen | ✅ `src/codegen.cpp` |
| **Assembly** | `.s` assembly | Object (`.o`) | `as` / `gcc -c` | ❌ External |
| **Linking** | `.o` + libs | Executable | `ld` / `gcc` | ❌ External |
| **Loading** | ELF executable | Running process | Kernel | ❌ External |

### What Happens When You Run `gcc file.c`

```
1. Preprocessor (cpp)     →  Expands #include, #define, #ifdef
2. Compiler (cc1)         →  Converts C → assembly (.s)
3. Assembler (as)         →  Converts assembly → object (.o)
4. Linker (ld)            →  Combines objects + libraries → executable
```

**This project implements stage 2 only** (C → assembly). Stages 3-4 are handled by the system `gcc`/`as`/`ld`.

## Supported C Subset

### Literals

| Feature | Lexer | Parser | Codegen | Lesson |
|---------|-------|--------|---------|--------|
| Integer (decimal) | ✅ | ✅ | ✅ | 0001 |
| Integer (hex `0xFF`) | ✅ | ✅ | ✅ | 0001 |
| Integer (binary `0b1010`) | ✅ | ✅ | ✅ | 0001 |
| Integer (octal `0777`) | ✅ | ✅ | ✅ | 0001 |
| Char (`'a'`, `'\n'`) | ✅ | ✅ | ✅ | 0001 |
| String (`"hello"`) | ✅ | ✅ | ✅ | 0019 |
| Float (`3.14`, `3.14f`) | ✅ | ✅ | ⚠️ Parsed | 0043 |
| Hex float (`0x1.0p10`) | ❌ | — | — | — |

### Types

| Feature | Lexer | Parser | Codegen | Lesson |
|---------|-------|--------|---------|--------|
| `int` | ✅ | ✅ | ✅ | 0013 |
| `char` | ✅ | ✅ | ✅ | 0013 |
| `void` | ✅ | ✅ | ✅ | 0013 |
| `bool` / `_Bool` | ✅ | ✅ | ✅ | 0010 |
| `float` | ✅ | ✅ | ⚠️ Parsed | 0043 |
| `double` | ✅ | ✅ | ⚠️ Parsed | 0043 |
| `long` / `long long` | ✅ | ✅ | ✅ | 0015 |
| `short` | ✅ | ✅ | ✅ | 0015 |
| `signed` / `unsigned` | ✅ | ✅ | ✅ | 0015 |
| Pointer types (`int*`) | ✅ | ✅ | ✅ | 0024 |
| Array types (`int[]`) | ✅ | ✅ | ✅ | 0025 |
| `struct` | ✅ | ✅ | ✅ | 0022-0023 |
| `union` | ✅ | ✅ | ✅ | 0027 |
| `enum` | ✅ | ✅ | ✅ | 0028 |
| `typedef` | ✅ | ✅ | ✅ | 0029 |
| `sizeof` | ✅ | ✅ | ✅ | 0014 |
| Function pointers | ✅ | ⚠️ Basic | ⚠️ Basic | 0036 |
| `size_t` | ❌ | — | — | — |
| `NULL` | ❌ | — | — | — |
| `void *` | ✅ | ✅ | ✅ | 0037 |
| Flexible array members | ❌ | — | — | — |

### Qualifiers & Storage

| Feature | Lexer | Parser | Codegen | Lesson |
|---------|-------|--------|---------|--------|
| `const` | ✅ | ✅ | ✅ | 0012 |
| `volatile` | ✅ | ✅ | ✅ | 0051 |
| `static` | ✅ | ✅ | ✅ | 0050 |
| `extern` | ✅ | ✅ | ✅ | 0021 |
| `inline` | ✅ | ✅ | ✅ | 0069 |
| `register` | ❌ | — | — | — |
| `auto` | ❌ | — | — | — |
| `restrict` | ❌ | — | — | — |
| `_Thread_local` | ❌ | — | — | — |
| `_Atomic` | ❌ | — | — | — |

### Operators

| Feature | Lexer | Parser | Codegen | Lesson |
|---------|-------|--------|---------|--------|
| Arithmetic (`+`, `-`, `*`, `/`, `%`) | ✅ | ✅ | ✅ | 0001 |
| Comparison (`==`, `!=`, `<`, `>`, `<=`, `>=`) | ✅ | ✅ | ✅ | 0001 |
| Logical (`&&`, `\|\|`, `!`) | ✅ | ✅ | ✅ | 0001 |
| Bitwise (`&`, `\|`, `^`, `~`, `<<`, `>>`) | ✅ | ✅ | ✅ | 0001 |
| Assignment (`=`) | ✅ | ✅ | ✅ | 0001 |
| Compound (`+=`, `-=`, `*=`, `/=`) | ✅ | ✅ | ✅ | 0006 |
| Increment/Decrement (`++`, `--`) | ✅ | ✅ | ✅ | 0006 |
| Ternary (`? :`) | ✅ | ✅ | ✅ | 0007 |
| Comma (`,`) | ✅ | ✅ | ✅ | 0009 |
| `sizeof` | ✅ | ✅ | ✅ | 0014 |
| Type casts (`(int)x`) | ✅ | ✅ | ✅ | 0016 |
| Address-of (`&`) | ✅ | ✅ | ✅ | 0024 |
| Dereference (`*`) | ✅ | ✅ | ✅ | 0024 |
| Member access (`.`) | ✅ | ✅ | ✅ | 0023 |
| Arrow (`->`) | ✅ | ✅ | ✅ | 0023 |
| Array indexing (`[]`) | ✅ | ✅ | ✅ | 0025 |

### Control Flow

| Feature | Lexer | Parser | Codegen | Lesson |
|---------|-------|--------|---------|--------|
| `if`/`else` | ✅ | ✅ | ✅ | 0001 |
| `while` | ✅ | ✅ | ✅ | 0001 |
| `do`/`while` | ✅ | ✅ | ✅ | 0008 |
| `for` | ✅ | ✅ | ✅ | 0001 |
| `switch`/`case`/`default` | ✅ | ✅ | ✅ | 0030 |
| `break` | ✅ | ✅ | ✅ | 0032 |
| `continue` | ✅ | ✅ | ✅ | 0032 |
| `goto` / labels | ✅ | ✅ | ✅ | 0031 |
| `return` | ✅ | ✅ | ✅ | 0001 |

### Functions

| Feature | Lexer | Parser | Codegen | Lesson |
|---------|-------|--------|---------|--------|
| Function declarations | ✅ | ✅ | ✅ | 0011 |
| Function definitions | ✅ | ✅ | ✅ | 0001 |
| Forward declarations | ✅ | ✅ | ✅ | 0011 |
| Parameters (6 reg ABI) | ✅ | ✅ | ✅ | 0001 |
| Recursive calls | ✅ | ✅ | ✅ | 0001 |
| Variadic (`...`) | ✅ | ✅ | ⚠️ Parsed | 0046 |
| Nested functions (GCC) | ❌ | — | — | — |

### Declarations & Initializers

| Feature | Lexer | Parser | Codegen | Lesson |
|---------|-------|--------|---------|--------|
| Variable declarations | ✅ | ✅ | ✅ | 0001 |
| Initializers (`int x = 5`) | ✅ | ✅ | ✅ | 0001 |
| Array init (`{1,2,3}`) | ✅ | ⚠️ Skipped | ⚠️ Zero-init | 0041 |
| Struct init (`{.x=1}`) | ✅ | ⚠️ Skipped | ⚠️ Zero-init | 0038 |
| Bitfields (`int x : 1`) | ✅ | ✅ | ⚠️ Parsed | 0040 |
| Multiple declarators (`int a,b`) | ❌ | — | — | — |

### Preprocessor

| Feature | Status | Lesson |
|---------|--------|--------|
| `#include` | ⚠️ Tracked | 0035 |
| `#define` (object-like) | ✅ Implemented | 0033 |
| `#define` (function-like) | ✅ Implemented | 0033 |
| `#ifdef` / `#ifndef` | ✅ Implemented | 0034 |
| `#if` / `#else` / `#endif` | ✅ Implemented | 0034 |
| `#undef` | ✅ Implemented | 0033 |
| `#pragma once` | ❌ Not implemented | — |
| `#error` | ✅ Implemented | 0033 |
| `#line` | ❌ Not implemented | — |
| Token pasting (`##`) | ❌ Not implemented | — |
| Stringification (`#`) | ❌ Not implemented | — |
| Variadic macros (`__VA_ARGS__`) | ❌ Not implemented | — |
| Nested macro expansion | ✅ Implemented | 0033 |

### Standard Library (Stubs)

| Feature | Status | Lesson |
|---------|--------|--------|
| `<stdio.h>` printf/scanf | ❌ External | 0054 |
| `<stdlib.h>` malloc/free | ❌ External | 0055 |
| `<string.h>` strlen/strcpy | ❌ External | 0053 |
| `<math.h>` sin/cos/sqrt | ❌ External | — |
| `<ctype.h>` isdigit/toupper | ❌ External | — |
| `<stdint.h>` int32_t/uint64_t | ❌ External | — |

### GCC Extensions

| Feature | Lexer | Parser | Codegen | Lesson |
|---------|-------|--------|---------|--------|
| Statement expressions `({...})` | ❌ | — | — | 0082 |
| Inline assembly `asm()` | ❌ | — | — | — |
| Label-as-value (`&&label`) | ❌ | — | — | 0083 |
| `__attribute__` | ❌ | — | — | 0084 |
| `__builtin_expect` | ❌ | — | — | 0085 |
| `__builtin_offsetof` | ❌ | — | — | — |
| Nested functions | ❌ | — | — | 0086 |
| Binary literals `0b1010` | ✅ | ✅ | ✅ | 0001 |

### C11 Features

| Feature | Status | Lesson |
|---------|--------|--------|
| `_Static_assert` | ❌ Not implemented | 0087 |
| `_Generic` | ❌ Not implemented | 0088 |
| `_Alignas` / `_Alignof` | ❌ Not implemented | 0089 |
| `_Atomic` | ❌ Not implemented | 0090 |
| `_Thread_local` | ❌ Not implemented | 0091 |
| Anonymous structs | ❌ Not implemented | 0092 |

### C17 Features

| Feature | Status | Lesson |
|---------|--------|--------|
| `__has_include` | ❌ Not implemented | — |
| `__has_builtin` | ❌ Not implemented | — |
| `__STDC_VERSION__` | ❌ Not implemented | — |

### C23 Features

| Feature | Status | Lesson |
|---------|--------|--------|
| `nullptr` | ❌ Not implemented | 3009 |
| `bool` / `true` / `false` literals | ❌ Not implemented | 3000 |
| `auto` type inference | ❌ Not implemented | 3001 |
| `constexpr` | ❌ Not implemented | 3008 |
| Digit separators (`1'000'000`) | ❌ Not implemented | 3004 |
| Binary literals (`0b1010`) | ✅ Implemented | 3005 |
| `[[attribute]]` syntax | ❌ Not implemented | 3007 |
| `#embed` | ❌ Not implemented | 3006 |

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
# Compile a C file to assembly (this project's output)
./build/simplecc -S input.c

# Then use external tools to produce executable:
gcc -o output input.s
./output; echo $?

# Or step by step:
as input.s -o input.o
ld input.o -o output -lc -dynamic-linker /lib64/ld-linux-x86-64.so.2
./output; echo $?
```

## Target Architecture

- **Platform:** x86-64 Linux
- **ABI:** System V ABI (Linux, macOS)
- **Calling convention:** First 6 integer args in rdi, rsi, rdx, rcx, r8, r9
- **Output:** GAS syntax assembly (`.s` files)
- **External tools:** GNU `as` (assembler), GNU `ld`/`gcc` (linker)

## Example

Input (`test.c`):
```c
struct Point { int x; int y; };

int main() {
    struct Point p;
    p.x = 10;
    p.y = 20;
    return p.x + p.y;
}
```

Compile and run:
```bash
./build/simplecc -S test.c -o test.s
gcc -o test test.s
./test; echo $?
# Output: 30
```

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

**Compilation status:** 60/70 lessons compile successfully.

### Core Lessons (0001-0005) — ✅ Complete

| Lesson | Topic | Tests | Compile |
|--------|-------|-------|---------|
| 0001 | Tokenizer (Lexer) | 26 | ✅ |
| 0002 | AST Definitions | 10 | ✅ |
| 0003 | Parser (Recursive Descent) | 20 | ✅ |
| 0004 | Code Generator (x86-64) | 11 | ✅ |
| 0005 | Integration (CLI, Pipeline) | 11 | ✅ |

### Quick Wins (0006-0012)

| Lesson | Topic | Compile |
|--------|-------|---------|
| 0006 | Compound Assignment | ✅ |
| 0007 | Ternary Operator | ✅ |
| 0008 | Do-While Loops | ✅ |
| 0009 | Comma Operator | ✅ |
| 0010 | Bool Type | ✅ |
| 0011 | Forward Declarations | ✅ |
| 0012 | const Qualifier | ✅ |

### Type System (0013-0018)

| Lesson | Topic | Compile |
|--------|-------|---------|
| 0013 | Type System | ✅ |
| 0014 | sizeof | ✅ |
| 0015 | Signed/Unsigned | ✅ |
| 0016 | Type Casts | ✅ |
| 0017 | Type Promotions | ✅ |
| 0018 | Type-Aware Codegen | ✅ |

### Strings & Globals (0019-0021)

| Lesson | Topic | Compile |
|--------|-------|---------|
| 0019 | String Literal Codegen | ✅ |
| 0020 | Global Variables | ✅ |
| 0021 | Extern Declarations | ✅ |

### Structs & Pointers (0022-0025)

| Lesson | Topic | Compile |
|--------|-------|---------|
| 0022 | Struct Declarations | ✅ |
| 0023 | Struct Access | ✅ |
| 0024 | Pointer Types | ✅ |
| 0025 | Array Types | ✅ |

### Advanced Features (0026-0032)

| Lesson | Topic | Compile |
|--------|-------|---------|
| 0026 | Pointer Arithmetic | ❌ |
| 0027 | Unions | ✅ |
| 0028 | Enums | ✅ |
| 0029 | Typedef | ✅ |
| 0030 | Switch/Case | ✅ |
| 0031 | Goto/Labels | ✅ |
| 0032 | Break/Continue | ✅ |

### Preprocessor (0033-0035)

| Lesson | Topic | Compile |
|--------|-------|---------|
| 0033 | Preprocessor Macros | ✅ |
| 0034 | Conditional Compilation | ✅ |
| 0035 | Include Directive | ✅ |

### Advanced C (0036-0045)

| Lesson | Topic | Compile |
|--------|-------|---------|
| 0036 | Function Pointers | ❌ |
| 0037 | Void Pointers | ❌ |
| 0038 | Designated Init | ❌ |
| 0039 | Compound Literals | ❌ |
| 0040 | Bitfields | ✅ |
| 0041 | Multi-Dim Arrays | ❌ |
| 0042 | Array-Pointer Decay | ❌ |
| 0043 | Float/Double | ❌ |
| 0044 | Static Assert | ✅ |
| 0045 | Generic | ❌ |

### System & Functions (0046-0065)

| Lesson | Topic | Compile |
|--------|-------|---------|
| 0046 | Variadic | ✅ |
| 0047 | Statement Expr | ❌ |
| 0048 | Inline ASM | ❌ |
| 0049 | Multi-File | ✅ |
| 0050 | Static Linkage | ✅ |
| 0051 | Volatile Qualifier | ✅ |
| 0052 | Syscalls | ✅ |
| 0053 | String Funcs | ✅ |
| 0054 | IO Functions | ✅ |
| 0055 | Memory Alloc | ✅ |
| 0056 | File IO | ✅ |
| 0057 | Process Control | ✅ |
| 0058 | Socket Prog | ✅ |
| 0059 | Signal Handling | ❌ |
| 0060 | Environment | ✅ |
| 0061 | Thread Support | ✅ |
| 0062 | Synchronization | ✅ |
| 0063 | Memory Mapping | ✅ |
| 0064 | Dynamic Loading | ✅ |
| 0065 | IPC | ✅ |

### Optimization & Self-Hosting (0066-0075)

| Lesson | Topic | Compile |
|--------|-------|---------|
| 0066 | Const Folding | ✅ |
| 0067 | Dead Code Elim | ✅ |
| 0068 | Register Alloc | ✅ |
| 0069 | Function Inline | ✅ |
| 0070 | Debug Info | ✅ |
| 0071 | Self-Host Prep | ✅ |
| 0072 | Compile Compiler 1 | ✅ |
| 0073 | Compile Compiler 2 | ✅ |
| 0074 | Compile Compiler 3 | ✅ |
| 0075 | Bootstrap | ✅ |

### GCC Extensions & C11 Features (0076-0092)

| Lesson | Topic | Compile |
|--------|-------|---------|
| 0076 | Register Auto | ✅ |
| 0077 | Restrict Qualifier | ✅ |
| 0078 | Variadic Macros | ✅ |
| 0079 | Token Pasting | ✅ |
| 0080 | Pragma | ✅ |
| 0081 | Standard Lib Stubs | ✅ |
| 0082 | Statement Expr (GCC) | ❌ |
| 0083 | Label-as-Value | ❌ |
| 0084 | Attribute (GCC) | ❌ |
| 0085 | Builtin Functions | ❌ |
| 0086 | Nested Functions | ❌ |
| 0087 | Static Assert (C11) | ❌ |
| 0088 | Generic Selection | ❌ |
| 0089 | Alignas/Alignof | ❌ |
| 0090 | Atomic (C11) | ❌ |
| 0091 | Thread Local | ❌ |
| 0092 | Anonymous Structs | ❌ |

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

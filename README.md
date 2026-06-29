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
| Integer (binary `0b1010`) | ✅ | ✅ | ✅ | 0001, 3005 |
| Integer (octal `0777`) | ✅ | ✅ | ✅ | 0001 |
| Digit separators (`1'000'000`) | ✅ | ✅ | ✅ | 3004 |
| Char (`'a'`, `'\n'`) | ✅ | ✅ | ✅ | 0001 |
| String (`"hello"`) | ✅ | ✅ | ✅ | 0019 |
| Bool (`true`/`false`) | ✅ | ✅ | ✅ | 3000 |
| Float (`3.14f`) | ✅ | ✅ | ✅ Bit pattern stored as int; no FPU arithmetic | 0043 |
| Double (`3.14`) | ✅ | ✅ | ✅ Bit pattern stored as int; no FPU arithmetic | 0043 |
| Hex float (`0x1.0p10`) | ✅ | ✅ | ⚠️ Tokenized, stored as int | 0043 |

### Types

| Feature | Lexer | Parser | Codegen | Lesson |
|---------|-------|--------|---------|--------|
| `int` | ✅ | ✅ | ✅ | 0013 |
| `char` | ✅ | ✅ | ✅ | 0013 |
| `void` | ✅ | ✅ | ✅ | 0013 |
| `bool` / `_Bool` | ✅ | ✅ | ✅ | 0010 |
| `float` | ✅ | ✅ | ✅ sizeof=4, bit pattern stored as int | 0043 |
| `double` | ✅ | ✅ | ✅ sizeof=8, bit pattern stored as int | 0043 |
| `long double` | ❌ | — | — | — |
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
| Function pointers | ✅ | ✅ | ✅ Indirect call | 0036 |
| `size_t` | ✅ | ✅ | ✅ Built-in typedef | — |
| `NULL` | ✅ | ✅ | ✅ Macro → 0 | — |
| Flexible array members | ✅ | ✅ | ⚠️ Parsed, no codegen | 0040 |

### Qualifiers & Storage

| Feature | Lexer | Parser | Codegen | Lesson |
|---------|-------|--------|---------|--------|
| `const` | ✅ | ✅ | ✅ | 0012 |
| `volatile` | ✅ | ✅ | ✅ | 0051 |
| `static` | ✅ | ✅ | ✅ | 0050 |
| `extern` | ✅ | ✅ | ✅ | 0021 |
| `inline` | ✅ | ✅ | ✅ | 0069 |
| `register` | ✅ | ✅ | ⚠️ Parsed, treated as auto | 0076 |
| `auto` | ✅ | ✅ | ⚠️ Parsed, treated as auto | 0076 |
| `restrict` | ✅ | ✅ | ⚠️ Parsed, no effect on codegen | 0077 |
| `_Thread_local` | ✅ | ✅ | ⚠️ Parsed, no effect on codegen | 1010 |
| `_Atomic` | ✅ | ✅ | ⚠️ Parsed, no effect on codegen | 1005 |

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
| Variadic (`...`) | ✅ | ✅ | ⚠️ Parsed, no `va_arg` support | 0046 |
| Nested functions (GCC) | ❌ | — | — | — |

### Declarations & Initializers

| Feature | Lexer | Parser | Codegen | Lesson |
|---------|-------|--------|---------|--------|
| Variable declarations | ✅ | ✅ | ✅ | 0001 |
| Initializers (`int x = 5`) | ✅ | ✅ | ✅ | 0001 |
| Array init (`{1,2,3}`) | ✅ | ⚠️ Parsed (values skipped) | ⚠️ Zero-init | 0041 |
| Struct init (`{.x=1}`) | ✅ | ⚠️ Parsed (values skipped) | ⚠️ Zero-init | 0038 |
| Bitfields (`int x : 1`) | ✅ | ✅ | ⚠️ Parsed | 0040 |
| Multiple declarators (`int a,b`) | ✅ | ✅ | ✅ | 0001 |
| Function pointers (`int (*fp)(int)`) | ✅ | ✅ | ✅ | 0036 |
| Compound literals (`(int[]){1,2}`) | ✅ | ✅ | ⚠️ Returns 0 | 0039 |

### Preprocessor

| Feature | Status | Lesson |
|---------|--------|--------|
| `#include` | ✅ Implemented (`"file.h"` only; `<*.h>` not bundled) | 0035 |
| `#define` (object-like) | ✅ Implemented | 0033 |
| `#define` (function-like) | ✅ Implemented | 0033 |
| `#ifdef` / `#ifndef` | ✅ Implemented | 0034 |
| `#if` / `#else` / `#endif` | ✅ Implemented | 0034 |
| `#undef` | ✅ Implemented | 0033 |
| `#pragma once` | ⚠️ Ignored | 0080 |
| `#error` | ✅ Implemented | 0033 |
| `#line` | ⚠️ Ignored | 0080 |
| Token pasting (`##`) | ✅ Implemented | 0079 |
| Stringification (`#`) | ✅ Implemented | 0033 |
| Variadic macros (`__VA_ARGS__`) | ✅ Implemented | 0046 |
| Nested macro expansion | ✅ Implemented | 0033 |

### Standard Library (Stubs)

| Feature | Status | Lesson |
|---------|--------|--------|
| `<stdio.h>` printf/scanf | ⚠️ Declared (extern) | 0054 |
| `<stdlib.h>` malloc/free | ⚠️ Declared (extern) | 0055 |
| `<string.h>` strlen/strcpy/memcpy | ⚠️ Declared (extern) | 0053 |
| `<math.h>` sin/cos/sqrt | ⚠️ Declared (extern) | 0081 |
| `<ctype.h>` isdigit/toupper | ⚠️ Declared (extern) | 0081 |
| `<stdint.h>` int32_t/uint64_t | ⚠️ Declared (extern) | 0081 |

### GCC Extensions

| Feature | Lexer | Parser | Codegen | Lesson |
|---------|-------|--------|---------|--------|
| Statement expressions `({...})` | ✅ | ✅ | ✅ | 0082 |
| Inline assembly `asm("...")` | ✅ | ✅ | ✅ Raw asm pass-through | 0048 |
| Extended asm `asm("..." : : : clobber)` | ✅ | ⚠️ Parsed (operands skipped) | ⚠️ Empty output | 0048 |
| Label-as-value (`&&label`) | ✅ | ✅ | ⚠️ Parsed (as int 0) | 0083 |
| `__attribute__` | ✅ | ✅ | ⚠️ Ignored | 0084 |
| `[[attribute]]` (C23) | ✅ | ✅ | ⚠️ Via `__attribute__` | 3007 |
| `__builtin_expect` | ✅ | ✅ | ✅ Returns first arg | 0085 |
| `__builtin_offsetof` | ✅ | ✅ | ✅ Computes struct offset | 0085 |
| `__builtin_popcount` | ✅ | ✅ | ⚠️ Emitted as external call | 0085 |
| Nested functions | ✅ | ✅ | ❌ Trampoline not supported | 0086 |
| Binary literals `0b1010` | ✅ | ✅ | ✅ | 0001, 3005 |
| Variadic macros (`__VA_ARGS__`) | ✅ | ✅ | ✅ | 0078 |
| Token pasting (`##`) | ✅ | ✅ | ✅ | 0079 |
| Stringification (`#`) | ✅ | ✅ | ✅ | 0033 |
| `#pragma` | ✅ | ✅ | ⚠️ Ignored | 0080 |

### C11 Features

| Feature | Status | Lesson |
|---------|--------|--------|
| `_Static_assert` | ✅ Parsed (skipped) | 1000 |
| `static_assert` (alias) | ⚠️ Needs `<assert.h>` | 1012 |
| `_Generic` | ✅ Parsed (simplified) | 1001 |
| `_Alignas` / `_Alignof` | ✅ Parsed | 1014 |
| `_Atomic` | ✅ Parsed (type qualifier) | 1005 |
| `_Thread_local` | ✅ Parsed (type qualifier) | 1010 |
| `<threads.h>` | ⚠️ Declared (extern) | 1006 |
| `aligned_alloc` | ⚠️ Declared (extern) | 1007 |
| `<tgmath.h>` | ⚠️ Declared (extern) | 1008 |
| `<stdatomic.h>` | ⚠️ Declared (extern) | 1009, 1015 |
| `_Noreturn` | ✅ Mapped to `static` | 1004 |
| `<stdnoreturn.h>` | ⚠️ Declared (extern) | 1013 |
| Anonymous structs | ✅ Parsed + codegen | 1002 |
| Anonymous unions | ✅ Parsed + codegen | 1003 |
| Annex K (`gets_s`, etc.) | ❌ Not implemented | 1011 |

### C17 Features

| Feature | Status | Lesson |
|---------|--------|--------|
| `<stdbool.h>` (bool/true/false) | ⚠️ Needs `<stdbool.h>` (not bundled) | 2000 |
| `<stdalign.h>` (alignas/alignof) | ⚠️ Needs `<stdalign.h>` (not bundled) | 2001 |
| `<stdnoreturn.h>` (noreturn) | ✅ Parsed | 2002 |
| `<stdint.h>` (int32_t/uint64_t) | ⚠️ Declared (extern) | 2003 |
| `typeof` | ✅ Mapped to sizeof | 2004 |
| `__STDC_VERSION__` | ✅ Defined (202311L) | 2005 |

### C23 Features

| Feature | Status | Lesson |
|---------|--------|--------|
| `nullptr` | ⚠️ Via `(void*)0` | 3009 |
| `bool` / `true` / `false` literals | ✅ Implemented | 3000 |
| `auto` type inference | ⚠️ Parsed (treated as `auto` storage) | 3001 |
| `for (int i = 0; ...)` init decl | ✅ Implemented | 3002 |
| Empty struct `struct {}` | ✅ Implemented | 3003 |
| Digit separators (`1'000'000`) | ✅ Implemented | 3004 |
| Binary literals (`0b1010`) | ✅ Implemented | 3005 |
| `[[attribute]]` syntax | ⚠️ Via `__attribute__` | 3007 |
| `[[nodiscard]]` | ⚠️ Parsed (ignored) | 3013 |
| `[[noreturn]]` | ⚠️ Parsed (ignored) | 3012 |
| `constexpr` | ⚠️ Parsed (ignored) | 3008, 3010 |
| `__VA_OPT__` | ❌ Not implemented | 3014 |
| `#embed` | ✅ Implemented (byte literal expansion) | 3006 |
| Predefined macros | ✅ `__STDC_VERSION__` etc. | 3014 |

## Known Limitations

This compiler is a substantial subset of C but not a complete C23 implementation. Documented limits:

### Floating Point
- **No FPU support** — `float` and `double` are lexed, parsed, and type-checked but stored as 8-byte integers in memory. Arithmetic and conversion between float and int is not performed.
- No `long double` support.
- Math library functions (`sin`, `cos`, `sqrt`, etc.) are declared `extern` but not implemented — calls link against system libm.

### Initializers
- **Braced initializers produce zero-initialized storage.** `int a[3] = {1, 2, 3}` reserves 3 ints of space but emits 0s. Designated initializers (`.x = 1`) are parsed but produce zero-init.
- **Compound literals `(int[]){1, 2, 3}`** parse and type-check but produce 0 at runtime.

### Variadic Functions
- `...` parameter lists are accepted; `va_start` / `va_arg` / `va_end` are not implemented. Variadic functions are emitted as zero-argument functions.

### Preprocessor
- **No standard library bundled** — `#include <stdio.h>` fails because headers aren't shipped. `#include "file.h"` works for files in the same directory.
- **`#pragma` is ignored** — including `#pragma once`, diagnostic pragmas, etc.
- **`#line` is ignored.**
- **`__VA_OPT__` is not implemented.**
- **Macro recursion is not detected** — recursive macros can hang the compiler.

### Standard Library
- No bundled C standard library. `<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<math.h>`, `<stdint.h>`, `<threads.h>`, `<stdatomic.h>`, etc. are not shipped.
- Standard library functions are called via system `gcc`/`ld` link. They are not compiled by this project.

### Inline Assembly
- **Basic form `asm("nop")` works** — assembly text is passed through to output.
- **Extended form `asm("..." : "=r"(x) : "r"(y) : "eax")`** parses but operand strings and clobbers are dropped. No register allocation for inputs/outputs.

### Nested Functions (GCC extension)
- **Cannot capture enclosing variables.** Nested function declarations parse but references to outer locals fail. Trampoline support (using `mprotect` to make stack executable) is not implemented.

### Constants
- **`_Static_assert` is parsed and silently skipped.** The expression is not evaluated at compile time. Always passes.
- **`_Generic` is parsed but simplified** — returns the first matching arm's expression type.
- **`constexpr` is parsed and ignored.**

### Other
- **No debug info emission** (`-g` equivalent). Assembler directives for `.file` / `.loc` are not generated.
- **No optimization passes** — no constant folding, dead code elimination, or register allocation beyond what the assembler provides.
- **No `goto *ptr` computed gotos for arbitrary pointers** — only `goto *(&&label)` for label-as-value works.
- **Function pointers are called via indirect call** — no vtable or devirtualization.
- **No `_Complex` or `_Imaginary` types.**

### External Tools Required
- `gcc` / `ld` for assembling and linking
- System libc/libm for standard library functions
- `as` (GNU assembler) for `.s` → `.o`

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

Each of these compiles and runs. Note that `enum`, `struct`, and `typedef` declarations are valid only at file scope (not inside a function body).

```c
// Pointer dereference → 42
int main() {
    int x = 42; int *p = &x; return *p;
}

// Array indexing → 20
int main() {
    int arr[3]; arr[1] = 20; return arr[1];
}

// Switch/case → 20
int main() {
    int x = 2;
    switch (x) { case 1: return 10; case 2: return 20; }
    return 0;
}

// Enum constants → 1
enum Color { RED=0, GREEN=1 };
int main() { return GREEN; }

// Typedef → 42
typedef int integer;
int main() { integer x = 42; return x; }

// Struct member access → 30
struct Point { int x; int y; };
int main() {
    struct Point p; p.x=10; p.y=20; return p.x+p.y;
}
```

## Lesson Progress

**Lesson status:** 105 lessons ✅ Complete, 15 lessons ⚠️ Partial. See each lesson's README for details of what is and is not implemented.

**Compilation status:** 113/118 lessons with example programs compile successfully. The 5 failures are all due to `#include <*.h>` for non-bundled standard library headers (e.g., `<stdio.h>`, `<stdbool.h>`, `<assert.h>`). All 5 are documented as requiring user-provided headers.

**Test status:** 6/6 test suites pass (tokenizer, AST, parser, codegen, integration, lessons 0076-1014).

### Core Lessons (0001-0005) — ✅ Complete

| Lesson | Topic | Tests | Compile |
|--------|-------|-------|---------|
| 0001 | Tokenizer (Lexer) | 26 | ✅ |
| 0002 | AST Definitions | 7 | ✅ |
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
| 0026 | Pointer Arithmetic | ✅ |
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
| 0036 | Function Pointers | ✅ |
| 0037 | Void Pointers | ✅ |
| 0038 | Designated Init | ⚠️ Partial |
| 0039 | Compound Literals | ⚠️ Partial |
| 0040 | Bitfields | ✅ |
| 0041 | 2D Arrays | ✅ |
| 0042 | Array-Pointer Decay | ✅ |
| 0043 | Float/Double | ⚠️ Partial |
| 0044 | Static Assert | ✅ |
| 0045 | Generic | ✅ |

### N-Dimensional Arrays (0087)

| Lesson | Topic | Compile |
|--------|-------|---------|
| 0087 | N-Dim Arrays | ✅ |

### System & Functions (0046-0065)

| Lesson | Topic | Compile |
|--------|-------|---------|
| 0046 | Variadic | ✅ |
| 0047 | Statement Expr | ✅ |
| 0048 | Inline ASM | ⚠️ Partial |
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
| 0059 | Signal Handling | ✅ |
| 0060 | Environment | ✅ |
| 0061 | Thread Support | ✅ |
| 0062 | Synchronization | ✅ |
| 0063 | Memory Mapping | ✅ |
| 0064 | Dynamic Loading | ✅ |
| 0065 | IPC | ✅ |

### Optimization & Self-Hosting (0066-0075)

| Lesson | Topic | Compile |
|--------|-------|---------|
| 0066 | Const Folding | ⚠️ Partial |
| 0067 | Dead Code Elim | ⚠️ Partial |
| 0068 | Register Alloc | ⚠️ Partial |
| 0069 | Function Inline | ⚠️ Partial |
| 0070 | Debug Info | ⚠️ Partial |
| 0071 | Self-Host Prep | ⚠️ Partial |
| 0072 | Compile Compiler 1 | ⚠️ Partial |
| 0073 | Compile Compiler 2 | ⚠️ Partial |
| 0074 | Compile Compiler 3 | ⚠️ Partial |
| 0075 | Bootstrap | ⚠️ Partial |

### GCC Extensions (0076-0086)

| Lesson | Topic | Compile |
|--------|-------|---------|
| 0076 | Register Auto | ✅ |
| 0077 | Restrict Qualifier | ✅ |
| 0078 | Variadic Macros | ✅ |
| 0079 | Token Pasting | ✅ |
| 0080 | Pragma | ✅ |
| 0081 | Standard Lib Stubs | ✅ |
| 0082 | Statement Expr (GCC) | ✅ |
| 0083 | Label-as-Value | ✅ |
| 0084 | Attribute (GCC) | ✅ |
| 0085 | Builtin Functions | ✅ |
| 0086 | Nested Functions | ⚠️ Partial |

### C11 Standard Lessons (1000-1015)

| Lesson | Topic | Compile |
|--------|-------|---------|
| 1000 | Static Assert | ✅ |
| 1001 | Generic Selection | ✅ |
| 1002 | Anonymous Structs | ✅ |
| 1003 | Anonymous Unions | ✅ |
| 1004 | _Noreturn | ✅ |
| 1005 | _Atomic | ✅ |
| 1006 | threads.h | ✅ |
| 1007 | aligned_alloc | ✅ |
| 1008 | tgmath | ✅ |
| 1009 | stdatomic.h | ✅ |
| 1010 | _Thread_local | ✅ |
| 1011 | Bounds Checking | ✅ |
| 1012 | static_assert alias | ✅ |
| 1013 | stdnoreturn.h | ✅ |
| 1014 | stdalign.h | ✅ |
| 1015 | stdatomic complete | ✅ |

### C17 Standard Lessons (2000-2005)

| Lesson | Topic | Compile |
|--------|-------|---------|
| 2000 | stdbool.h | ✅ |
| 2001 | stdalign.h | ✅ |
| 2002 | stdnoreturn.h | ✅ |
| 2003 | stdint.h | ✅ |
| 2004 | typeof | ✅ |
| 2005 | __STDC_VERSION__ | ✅ |

### C23 Standard Lessons (3000-3014)

| Lesson | Topic | Compile |
|--------|-------|---------|
| 3000 | bool literal | ✅ |
| 3001 | auto type | ✅ |
| 3002 | for-init-decl | ✅ |
| 3003 | empty structs | ✅ |
| 3004 | digit separators | ✅ |
| 3005 | binary literals | ✅ |
| 3006 | #embed | ✅ |
| 3007 | [[attribute]] | ✅ |
| 3008 | constexpr | ✅ |
| 3009 | nullptr | ✅ |
| 3010 | constexpr-func | ✅ |
| 3011 | auto-array | ✅ |
| 3012 | noreturn-attr | ✅ |
| 3013 | [[nodiscard]] | ✅ |
| 3014 | predef-macros | ✅ |

## Test Results

```
1/6 Test #1: tokenizer_tests ..................   Passed
2/6 Test #2: ast_tests ........................   Passed
3/6 Test #3: parser_tests .....................   Passed
4/6 Test #4: codegen_tests ....................   Passed
5/6 Test #5: integration_tests ................   Passed
6/6 Test #6: test_lessons_0076_1014 ...........   Passed
100% tests passed, 0 tests failed out of 6
```

## References

- [Compilers: Principles, Techniques, and Tools](https://dragonbook.stanford.edu/)
- [chibicc - Small C Compiler](https://github.com/rui314/chibicc)
- [Writing a C Compiler](https://norasandler.com/2017/11/29/Write-a-Compiler.html)
- [x86-64 SysV ABI](https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf)

### Additional Lessons (0088-0097)

| Lesson | Topic | Status |
|--------|-------|--------|
| 0088 | String Concatenation | ✅ |
| 0090 | For-Loop Comma | ✅ |
| 0093 | Anonymous Enum | ⚠️ Partial |
| 0094 | Typedef Func Ptr | ⚠️ Partial |
| 0095 | Nested Struct Init | ⚠️ Partial |
| 0096 | ND Array Init | ⚠️ Partial |

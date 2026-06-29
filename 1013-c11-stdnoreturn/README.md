# Lesson 1013: Header `<stdnoreturn.h>` (C11)

## Status: ⚠️ Partial | Standard: C11 | Effort: Trivial

## Objective

Provide `noreturn` macro for `_Noreturn`.

## How It Works

simplecc ships **no** `<stdnoreturn.h>` in `lib/`. The macro form is provided by the lexer itself: the bare `noreturn` identifier is mapped directly to `KW_STATIC`, mirroring `_Noreturn` (lesson 1004):

```cpp
// src/lexer.cpp:148-149
{"_Noreturn", TokenType::KW_STATIC}, // treat as storage class for now
{"noreturn",  TokenType::KW_STATIC},
```

So `noreturn void f();` is accepted with or without the header. The bundled example in `1013-c11-stdnoreturn/src/example.c` is a stub that does not actually use the qualifier.

## Usage (intended)

```c
#include <stdnoreturn.h>

noreturn void abort(void);
noreturn void exit(int status);
```

## What Works

| Feature | Status |
|---------|--------|
| `noreturn` accepted as keyword | ✅ |
| `<stdnoreturn.h>` shim | ❌ Not present in `lib/` (but unnecessary, the keyword works) |
| Real noreturn codegen | ❌ Treated as a no-op storage class (lesson 1004) |

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `noreturn` keyword | `src/lexer.cpp:149` | `noreturn` → `KW_STATIC` |
| Storage class | `src/parser.cpp:108-110` | Appended to type string |

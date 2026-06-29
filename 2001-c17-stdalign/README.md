# Lesson 2001: stdalign.h (C17)

## Status: ✅ Complete | Standard: C17 | Effort: Trivial

## Objective

Provide `alignas` and `alignof` macros.

## How It Works

The header shim maps the macro names to the C11 keyword tokens. The lexer also accepts the lower-case spellings directly, so the header is technically optional:

```c
// lib/stdalign.h
#define alignas _Alignas
#define alignof _Alignof
```

```cpp
// src/lexer.cpp:143-146
{"_Alignof", TokenType::KW_ALIGNOF},
{"alignof",  TokenType::KW_ALIGNOF},
{"_Alignas", TokenType::KW_ALIGNAS},
{"alignas",  TokenType::KW_ALIGNAS},
```

The example in `2001-c17-stdalign/src/example.c` does `#include <stdalign.h>` and evaluates `alignof(int)`, which the parser lowers to a `SizeofExprNode` that emits `mov $4, %rax` (see lesson 1014 and 1007).

## C17 Notes

- No changes from C11.
- `<stdalign.h>` provides: `alignas`, `alignof`.
- Maps to `_Alignas`, `_Alignof`.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Header shim | `lib/stdalign.h` | `#define alignas _Alignas` etc. |
| Lexer aliases | `src/lexer.cpp:143-146` | Both spellings accepted |
| `_Alignof` parse | `src/parser.cpp:1653-1672` | Reuses `SizeofExprNode` |

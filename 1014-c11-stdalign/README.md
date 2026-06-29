# Lesson 1014: Header `<stdalign.h>` (C11)

## Status: ✅ Complete | Standard: C11 | Effort: Trivial

## Objective

Provide `alignas` and `alignof` macros that expand to the C11 keywords.

## How It Works

Two layers cooperate:

1. The header shim maps the macro names to the keyword tokens.
2. The lexer also accepts the lower-case spellings as keywords directly, so the header is optional.

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

The C11 keywords themselves are handled in `parse_type_specifier` (skip `_Alignas`) and `parse_unary` (lower `_Alignof` to `SizeofExprNode`) — see lesson 1007 for the full walkthrough.

## Example

```c
#include <stdalign.h>

int main() {
    int x = alignof(int);  // returns 4
    return x;
}
```

## What Works

| Feature | Status |
|---------|--------|
| `alignof(int)` returns 4 | ✅ |
| `alignas(N)` parsed and skipped | ✅ |
| Real alignment-aware allocation | ❌ — no padding/alignment logic in codegen |

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Header shim | `lib/stdalign.h` | `#define alignas _Alignas` etc. |
| Lexer aliases | `src/lexer.cpp:143-146` | Both spellings accepted |
| `_Alignof` parse | `src/parser.cpp:1653-1672` | Reuses `SizeofExprNode` |

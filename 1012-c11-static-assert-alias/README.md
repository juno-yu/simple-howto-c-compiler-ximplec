# Lesson 1012: static_assert (C11 Optional)

## Status: ✅ Complete | Standard: C11 | Effort: Easy

## Objective

Alias `static_assert` to `_Static_assert`.

## How It Works

Both spellings map to the same token at lex time:

```cpp
// src/lexer.cpp:140-141
{"_Static_assert", TokenType::KW_STATIC_ASSERT},
{"static_assert",   TokenType::KW_STATIC_ASSERT},
```

The parser then routes to `parse_static_assert` exactly as in C11 (lesson 1000). C11 only requires `_Static_assert` as a keyword; `static_assert` is optionally provided as a macro in `<assert.h>`. simplecc promotes it to a real keyword, so the `#include <assert.h>` in the example is not strictly required for the macro form.

```c
// src/1012-c11-static-assert-alias/src/example.c
#include <assert.h>
int main() {
    static_assert(sizeof(int) == 4, "int must be 4 bytes");
    return 0;
}
```

## Notes

- `<assert.h>` in `lib/assert.h` defines a runtime `assert(expr)` macro but does **not** define a `static_assert` macro — the keyword form is the only path.
- C23 makes `static_assert` a keyword by default.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Token mapping | `src/lexer.cpp:140-141` | Both spellings → `KW_STATIC_ASSERT` |
| Parser dispatch | `src/parser.cpp:1069-1071` | Routes to `parse_static_assert` |

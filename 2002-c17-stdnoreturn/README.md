# Lesson 2002: stdnoreturn.h (C17)

## Status: ⚠️ Partial | Standard: C17 | Effort: Trivial

## Objective

Provide `noreturn` macro.

## How It Works

simplecc ships **no** `<stdnoreturn.h>` in `lib/`. The keyword form is recognised by the lexer directly:

```cpp
// src/lexer.cpp:148-149
{"_Noreturn", TokenType::KW_STATIC}, // treat as storage class for now
{"noreturn",  TokenType::KW_STATIC},
```

so a header shim is unnecessary. The bundled example in `2002-c17-stdnoreturn/src/example.c` is a stub that does not use the qualifier.

## C17 Notes

- No changes from C11.
- `<stdnoreturn.h>` provides: `noreturn`.
- Maps to `_Noreturn`.

## Limitations

- No real noreturn codegen — see lesson 1004.

## Source Code References

| Component | File | Status |
|-----------|------|--------|
| `<stdnoreturn.h>` | `lib/` | ❌ Not present (unnecessary, the keyword works) |
| `noreturn` keyword | `src/lexer.cpp:149` | `noreturn` → `KW_STATIC` |

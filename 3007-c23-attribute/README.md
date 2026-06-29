# Lesson 3007: Attributes (C23)

## Status: ⚠️ Partial | Standard: C23 | Effort: Medium

## Objective

Standardised attribute syntax `[[attr]]`.

## How It Works

simplecc does **not** recognise C23 `[[...]]` syntax — the lexer has no `[[` token. The GCC `__attribute__((...))` form, however, is recognised and silently discarded:

```cpp
// src/lexer.cpp:147
{"__attribute__", TokenType::KW_ATTRIBUTE},
```

```cpp
// src/parser.cpp:129-138
} else if (match(TokenType::KW_ATTRIBUTE)) {
    // __attribute__((...)) — skip entirely
    if (match(TokenType::LPAREN)) {
        if (match(TokenType::LPAREN)) {
            // Skip until ))
            int depth = 1;
            while (!is_at_end()) {
                if (check(TokenType::LPAREN)) depth++;
                else if (check(TokenType::RPAREN)) {
                    depth--;
                    if (depth == 0) { advance(); break; }
                }
                advance();
            }
        }
    }
}
```

A second consumer in the declaration path also strips the attribute before the declaration continues:

```cpp
// src/parser.cpp:623-625
// Skip __attribute__((...)) if present
while (check(TokenType::KW_ATTRIBUTE)) {
    advance(); // consume __attribute__
    // ... skip ( ( ... ) ) ...
}
```

The bundled example in `3007-c23-attribute/src/example.c` does not use either form; it is a stub that returns `0`.

## Standard Attributes (aspirational)

| Attribute | Purpose |
|-----------|---------|
| `[[noreturn]]` | Function never returns |
| `[[nodiscard]]` | Warn if return ignored |
| `[[maybe_unused]]` | Suppress unused warning |
| `[[deprecated]]` | Mark as deprecated |
| `[[fallthrough]]` | Suppress fallthrough warning |

## What Works

| Feature | Status |
|---------|--------|
| `__attribute__((...))` parsed and discarded | ✅ |
| `[[noreturn]]` etc. | ❌ Lexer does not recognise `[[` |

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|---|
| `__attribute__` keyword | `src/lexer.cpp:147` | `__attribute__` → `KW_ATTRIBUTE` |
| Attribute skip in type-spec | `src/parser.cpp:129-138` | Discards specifier |
| Attribute skip in decl | `src/parser.cpp:623-625` | Discards before decl |

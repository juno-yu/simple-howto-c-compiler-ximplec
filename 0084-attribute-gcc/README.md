# Lesson 0084: `__attribute__((...))` (GCC Extension)

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Easy

## Objective

Recognise and ignore the GCC `__attribute__` syntax. Attributes
provide hints to the compiler (alignment, format strings, unused,
etc.) and are not part of standard C. Unknown attributes should be
silently accepted.

## Syntax

```c
int x __attribute__((unused)) = 42;
void f(void) __attribute__((noreturn));
int printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
```

## Semantics

- `__attribute__((name))` or `__attribute__((name(args)))` may
  appear after a declaration.
- Standardized as `[[attr]]` in C23; the compiler accepts both
  forms.

## Implementation

The keyword is added to the lexer as `KW_ATTRIBUTE` and to the
type specifier chain in the parser. The parser consumes the entire
`((...))` group (counting parentheses) and discards the contents:

```cpp
// src/parser.cpp:129
} else if (match(TokenType::KW_ATTRIBUTE)) {
    // __attribute__((...)) — skip entirely
    if (match(TokenType::LPAREN)) {
        if (match(TokenType::LPAREN)) {
            // Skip until ))
            int depth = 1;
            while (depth > 0 && !is_at_end()) {
                if (check(TokenType::LPAREN)) depth++;
                if (check(TokenType::RPAREN)) depth--;
                if (depth > 0) advance();
            }
            if (!is_at_end()) advance(); // consume )
        }
        if (!is_at_end()) advance(); // consume )
    }
}
```

The same skip logic is applied in the variable-declaration path
(`parse_var_decl`) so that attributes appearing after the variable
name in a declarator are also dropped:

```cpp
// src/parser.cpp:623
// Skip __attribute__((...)) if present
while (check(TokenType::KW_ATTRIBUTE)) {
    advance(); // consume __attribute__
    if (match(TokenType::LPAREN)) {
        if (match(TokenType::LPAREN)) {
            int depth = 1;
            while (depth > 0 && !is_at_end()) {
                if (check(TokenType::LPAREN)) depth++;
                if (check(TokenType::RPAREN)) depth--;
                if (depth > 0) advance();
            }
            if (!is_at_end()) advance(); // consume ))
        }
        if (!is_at_end()) advance(); // consume )
    }
}
```

The codegen has no knowledge of the attribute — it is dropped
entirely before the AST node is built.

## What Works

- `__attribute__((name))` is accepted in type specifier position
  (before a declaration).
- `__attribute__((name(args)))` is accepted in declarator position
  (after a variable name).
- Nested parentheses inside the attribute are correctly counted.
- Multiple attributes can appear in sequence (e.g.
  `__attribute__((noreturn)) __attribute__((unused))`).

## Limitations

- **No attributes are honored.** Including:
  - `((unused))` — no warning suppression.
  - `((noreturn))` — no special codegen.
  - `((aligned(n)))` — no alignment adjustment.
  - `((format(...)))` — no format-string checking.
  - `((constructor))` / `((destructor))` — no automatic init/fini
    emission.
- **C23 `[[...]]` syntax** is a different feature (see lesson
  3007) and is recognised separately.

## Example

```c
int x __attribute__((unused)) = 42;
int main() {
    return 0;
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `KW_ATTRIBUTE` | `src/token.h:61` | Token type |
| Lexer keyword | `src/lexer.cpp:147` | `"__attribute__"` → `KW_ATTRIBUTE` |
| Token name | `src/lexer.cpp:50` | Maps `KW_ATTRIBUTE` to `"__attribute__"` |
| Type specifier | `src/parser.cpp:129-143` | Skips the `((...))` group |
| Declarator | `src/parser.cpp:623-638` | Skips `((...))` after a variable name |

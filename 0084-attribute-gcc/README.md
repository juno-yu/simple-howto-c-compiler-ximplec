# Lesson 0084: `__attribute__((...))` (GCC Extension)

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Easy

## Objective

Recognize and ignore the GCC `__attribute__` syntax. Attributes provide hints to the compiler (alignment, format strings, unused, etc.) and are not part of standard C. Unknown attributes should be silently accepted.

## Syntax

```c
int x __attribute__((unused)) = 42;
void f(void) __attribute__((noreturn));
int printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
```

## Semantics

- `__attribute__((name))` or `__attribute__((name(args)))` may appear after a declaration.
- Standardized as `[[attr]]` in C23; the compiler accepts both forms.

## Implementation

The preprocessor/parser detects `__attribute__` and skips the entire `((...))` group (counting parentheses). The codegen has no knowledge of the attribute — it is dropped.

## Limitations

- **No attributes are honored.** Including:
  - `((unused))` — no warning suppression.
  - `((noreturn))` — no special codegen.
  - `((aligned(n)))` — no alignment adjustment.
  - `((format(...))` — no format-string checking.
  - `((constructor))` / `((destructor))` — no automatic init/fini emission.
- **C23 `[[...]]` syntax** is recognized as `__attribute__` for compatibility but also not honored.

## Example

```c
int x __attribute__((unused)) = 42;
int main() {
    return 0;
}
```

## Source Code References

| Component | File | Description |
|-----------|------|-------------|
| Token | `src/token.h` | `KW_ATTRIBUTE` token type |
| Lexer | `src/lexer.cpp` | Maps `__attribute__` to `KW_ATTRIBUTE` |
| Parser | `src/parser.cpp` | Skips `__attribute__((...))` in declarators |
| Codegen | `src/codegen.cpp` | No special handling |

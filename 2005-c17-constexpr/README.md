# Lesson 2005: constexpr (C17 GCC Extension)

## Status: ⚠️ Skeleton | Standard: C17 (GCC extension) | Effort: Easy

## Objective

Compile-time constant expressions (GCC extension, standardised in C23).

## How It Works

simplecc has **no** `constexpr` keyword. Searching the lexer and parser for `constexpr` returns no matches:

```text
$ grep -R "constexpr" src/
(no matches)
```

The bundled example in `2005-c17-constexpr/src/example.c` is a stub that returns `42` without using the qualifier.

## Notes

- C17 doesn't standardise `constexpr`. GCC/Clang support `__attribute__((const))` and similar, but the example here does not exercise that path.
- C23 makes `constexpr` official (see lessons 3008 and 3010).
- No constant-expression validator, no array-size enforcement, no switch-case enforcement.

## Source Code References

| Component | File | Status |
|-----------|------|--------|
| `constexpr` keyword | `src/lexer.cpp` | ❌ Not defined |

# Lesson 3010: constexpr Functions (C23)

## Status: ⚠️ Skeleton | Standard: C23 | Effort: Medium

## Objective

Functions evaluable at compile time.

## How It Works

simplecc has **no** `constexpr` keyword on functions either (see lesson 3008). The bundled example in `3010-c23-constexpr-func/src/example.c` is a stub that returns `42` from a plain `int x = 42;`.

## Syntax (aspirational)

```c
constexpr int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

constexpr int fact10 = factorial(10);  // computed at compile time
```

## Limitations

- No `constexpr` keyword.
- No compile-time evaluation of constant calls.
- No fallback to runtime for non-constant arguments.

## Source Code References

| Component | File | Status |
|-----------|------|--------|
| `constexpr` keyword | `src/lexer.cpp` | ❌ Not defined |

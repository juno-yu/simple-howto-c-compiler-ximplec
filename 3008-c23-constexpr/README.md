# Lesson 3008: constexpr (C23)

## Status: ⚠️ Skeleton | Standard: C23 | Effort: Medium

## Objective

Compile-time constant expressions (C23).

## How It Works

simplecc has **no** `constexpr` keyword. Searching the lexer and parser for `constexpr` returns no matches:

```text
$ grep -R "constexpr" src/
(no matches)
```

The bundled example in `3008-c23-constexpr/src/example.c` is a stub that does not use the qualifier; it returns `42` from a plain `int x = 42;`.

## Syntax (aspirational)

```c
constexpr int size = 10;
constexpr int arr[size] = {0};
constexpr int square(int x) { return x * x; }
```

## Limitations

- No `constexpr` keyword.
- No constant-expression validator.
- No constexpr functions (see lesson 3010).

## Source Code References

| Component | File | Status |
|-----------|------|--------|
| `constexpr` keyword | `src/lexer.cpp` | ❌ Not defined |

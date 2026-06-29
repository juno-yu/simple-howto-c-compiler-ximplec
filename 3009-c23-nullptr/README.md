# Lesson 3009: nullptr (C23)

## Status: ❌ Not implemented | Standard: C23 | Effort: Easy

## Objective

Null pointer constant keyword.

## How It Works

simplecc does **not** recognise `nullptr` as a keyword. The lexer does not list it, and the preprocessor does not define a `nullptr` macro. As a result, an `int *p = nullptr;` declaration emits code that references a label named `nullptr` and the linker fails:

```text
$ simplecc -S test.c
$ gcc -o x test.c.s
ld: undefined reference to `nullptr'
```

The bundled example in `3009-c23-nullptr/src/example.c` works around this by using `(void*)0` directly and returns `1` from `p == (void*)0`.

## Syntax

```c
int *p = nullptr;      // simplecc: ❌ unresolved external
if (p == nullptr) { }  // simplecc: ❌ unresolved external
```

## C23 vs C11

| Feature | C11 | C23 |
|---------|-----|-----|
| Null pointer | `NULL` (macro) | `nullptr` (keyword) |
| Type safety | Weak | Strong |
| Usable in `_Generic` | No | Yes |

## What Works

| Feature | Status |
|---------|--------|
| `NULL` macro | ✅ (defined as `0` in `src/preprocessor.cpp:29` and `(void*)0` in `lib/stddef.h`) |
| `nullptr` keyword | ❌ |

## Source Code References

| Component | File | Status |
|-----------|------|--------|
| `nullptr` keyword | `src/lexer.cpp` | ❌ Not defined |
| `NULL` macro | `src/preprocessor.cpp:29` | Built-in macro `0` |
| `NULL` shim | `lib/stddef.h` | `#define NULL ((void*)0)` |

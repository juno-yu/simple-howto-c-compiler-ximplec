# Lesson 0077: `restrict` Qualifier

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Easy

## Objective

Support the C99 `restrict` type qualifier on pointer parameters. It is a promise from the programmer that, for the lifetime of the pointer, no other pointer will access the same object. This enables the compiler to perform optimizations that would otherwise be unsafe.

## Syntax

```c
void copy(int *restrict dst, const int *restrict src, int n);
```

## Semantics

- Applies only to pointer types (`int *restrict p`).
- A `restrict`-qualified pointer is the **only** pointer used to access the pointed-to object.
- Violating the contract is undefined behavior.

## Implementation

The `restrict` keyword is added to the lexer's keyword map as `KW_RESTRICT`. The parser consumes it inside the type-specifier chain and appends `restrict` to the type-name string. The codegen has no special treatment — `restrict` is purely a programmer-to-compiler annotation and is allowed to be a no-op at this optimization level.

## Limitations

- **No optimization effect.** The codegen does not exploit `restrict` to enable vectorization, redundancy elimination, or other pointer-aliasing-based optimizations.
- No diagnostic if the contract is violated by the source code.

## Example

```c
void copy(int *restrict dst, int *restrict src, int n) {
    for (int i = 0; i < n; i++) dst[i] = src[i];
}
int main() {
    int a[3] = {1, 2, 3};
    int b[3];
    copy(b, a, 3);
    return b[2];  // returns 3
}
```

## Source Code References

| Component | File | Description |
|-----------|------|-------------|
| Lexer keyword | `src/lexer.cpp` | `restrict` mapped to `KW_RESTRICT` |
| Type specifier | `src/parser.cpp` (`parse_type_specifier`) | Appends `restrict` to type name |
| Codegen | `src/codegen.cpp` | No special treatment (parsed, ignored) |

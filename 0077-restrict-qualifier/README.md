# Lesson 0077: `restrict` Qualifier

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Easy

## Objective

Support the C99 `restrict` type qualifier on pointer parameters. It
is a promise from the programmer that, for the lifetime of the
pointer, no other pointer will access the same object. This enables
the compiler to perform optimizations that would otherwise be unsafe.

## Syntax

```c
void copy(int *restrict dst, const int *restrict src, int n);
```

## Semantics

- Applies only to pointer types (`int *restrict p`).
- A `restrict`-qualified pointer is the **only** pointer used to
  access the pointed-to object.
- Violating the contract is undefined behavior.

## Implementation

The `restrict` keyword is added to the lexer's keyword map as
`KW_RESTRICT` and the parser consumes it inside the type-specifier
chain, appending `restrict` to the type-name string. The codegen
then strips it before any type arithmetic. There is no codegen
effect — `restrict` is purely a programmer-to-compiler annotation,
and at this optimization level it is treated as a no-op.

```cpp
// src/parser.cpp:116
} else if (match(TokenType::KW_RESTRICT)) {
    result += "restrict ";
}
```

After the full type string is built, the codegen strips `restrict`
along with the other qualifiers:

```cpp
// src/codegen.cpp:2117
strip("register"); strip("auto"); strip("inline"); strip("restrict");
```

## What Works

- `restrict` is accepted in any position in the type specifier.
- `restrict` is accepted in pointer chains (`int *restrict p`,
  `int *restrict *q`).
- Combinations such as `const restrict int *p` and
  `int *const restrict p` are accepted.

## Limitations

- **No optimization effect.** The codegen does not exploit
  `restrict` to enable vectorization, redundancy elimination, or
  other pointer-aliasing-based optimizations. (A real
  optimization pass could use this information to hoist redundant
  loads out of loops, etc.)
- No diagnostic is produced if the contract is violated by the
  source code.

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

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `KW_RESTRICT` | `src/token.h:54` | Token type |
| Lexer keyword | `src/lexer.cpp:137` | `"restrict"` → `KW_RESTRICT` |
| Token name | `src/lexer.cpp:43` | Maps `KW_RESTRICT` to `"restrict"` |
| `is_type_specifier` | `src/parser.cpp:80` | Recognises `KW_RESTRICT` as a type specifier |
| Type specifier chain | `src/parser.cpp:116-117` | Appends `restrict ` to the type string |
| Pointer chain | `src/parser.cpp:255-262` | Skips qualifiers after `*` (incl. `restrict`) |
| Codegen strip | `src/codegen.cpp:2117` | Strips `restrict` before type arithmetic |

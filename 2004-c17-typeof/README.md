# Lesson 2004: typeof (C17)

## Status: ✅ Complete (sizeof-equivalent) | Standard: C17 GCC extension | Effort: Easy

## Objective

Recognise the GCC `typeof` extension. Standardised in C23.

## How It Works

Both `typeof` and `__typeof__` are mapped to `KW_SIZEOF` by the lexer. The parser then reuses the sizeof path:

```cpp
// src/lexer.cpp:150-151
{"typeof",    TokenType::KW_SIZEOF}, // treat typeof as sizeof for now
{"__typeof__",TokenType::KW_SIZEOF},
```

The bundled example in `2004-c17-typeof/src/example.c` does not actually use `typeof`; it just exercises `int` arithmetic and returns 52. The keyword path is the same as the sizeof path: `parse_unary` sees `KW_SIZEOF`, parses the parenthesised operand, and produces a `SizeofExprNode` (lesson 1014).

## Notes

- C17 does not standardise `typeof`. This lesson covers the GCC extension.
- C23 makes it official.
- Because simplecc reuses the sizeof codegen, `typeof(int)` will yield `4` (the size of `int`), not a usable type alias. The keyword is parsed but not used for type inference.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Lexer mapping | `src/lexer.cpp:150-151` | `typeof`/`__typeof__` → `KW_SIZEOF` |
| SizeofExprNode codegen | `src/codegen.cpp:1120-1145` | Emits the byte count |

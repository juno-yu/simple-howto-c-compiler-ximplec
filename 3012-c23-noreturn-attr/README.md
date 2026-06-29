# Lesson 3012: [[noreturn]] Attribute (C23)

## Status: ⚠️ Partial | Standard: C23 | Effort: Easy

## Objective

Replace `_Noreturn` with attribute syntax.

## How It Works

simplecc does **not** recognise C23 `[[...]]` syntax — the lexer has no `[[` token (see lesson 3007). The bundled example in `3012-c23-noreturn-attr/src/example.c` is a stub that does not use the attribute; it returns `0` from `int main() { return 0; }`.

The C11 keyword `_Noreturn` is still recognised (lesson 1004) and behaves as a no-op storage class.

## Syntax (aspirational)

```c
[[noreturn]] void abort(void);
[[noreturn]] void exit(int);
```

## Limitations

- `[[noreturn]]` is **not** recognised — the lexer has no `[[` token.
- The C11 `_Noreturn` keyword is the only working spelling.
- No special-cased codegen (no `jmp` instead of `call`).

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|---|
| `[[` token | `src/lexer.cpp` | ❌ Not defined |
| `_Noreturn` keyword | `src/lexer.cpp:148` | Treated as storage class (lesson 1004) |

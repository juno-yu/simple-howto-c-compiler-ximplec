# Lesson 3013: [[nodiscard]] (C23)

## Status: ⚠️ Partial | Standard: C23 | Effort: Easy

## Objective

Warn when function return value is ignored.

## How It Works

simplecc does **not** recognise C23 `[[...]]` syntax — the lexer has no `[[` token (see lesson 3007). The bundled example in `3013-c23-nodiscard/src/example.c` is a stub that does not use the attribute; it returns `0` from `int main() { return 0; }`.

The GCC `__attribute__((warn_unused_result))` form is recognised by `__attribute__` skipping (lesson 3007) but the `warn_unused_result` name is not separately checked.

## Syntax (aspirational)

```c
[[nodiscard]] int critical_operation(void);
[[nodiscard("error code")]] int error_prone(void);

critical_operation();  // compiler warns
```

## Limitations

- `[[nodiscard]]` is **not** recognised.
- No unused-result warning is emitted in any path.
- `(void)f();` cast-to-void is parsed but no diagnostic is generated for missing casts.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|---|
| `[[` token | `src/lexer.cpp` | ❌ Not defined |
| `__attribute__` skipping | `src/parser.cpp:129-138` | Discards any attribute |

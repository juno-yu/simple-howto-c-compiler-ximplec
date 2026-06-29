# Lesson 1004: _Noreturn (C11)

## Status: ✅ Complete | Standard: C11 | Effort: Easy

## Objective

Mark functions that never return.

## Syntax

```c
_Noreturn void abort(void);
_Noreturn void exit(int status);
```

## How It Works

The lexer maps both `_Noreturn` and the C11 `<stdnoreturn.h>` spelling `noreturn` onto the existing `KW_STATIC` storage-class token. The storage class is then consumed by `parse_type_specifier` and the function declaration continues normally.

```cpp
// src/lexer.cpp:148-149
{"_Noreturn", TokenType::KW_STATIC}, // treat as storage class for now
{"noreturn",  TokenType::KW_STATIC},
```

No code is emitted to mark the function as noreturn; the qualifier is recorded as a string token in the type specifier and discarded during codegen. The example program in `1004-c11-noreturn/src/example.c` is a stub that does not actually call a noreturn function.

## Semantics (intended)

- Function does not return to caller
- Calling program must not rely on return value
- Compiler may optimize based on this
- Undefined behavior if function actually returns

## Limitations

- simplecc has no separate `noreturn` flag on functions — the qualifier is treated as a generic storage-class keyword and contributes nothing to codegen.
- No warning if a `_Noreturn` function has a `return` statement.
- No special-cased codegen (no `jmp` instead of `call`).

## Comparison with C23

| Feature | C11 | C23 |
|---------|-----|-----|
| Syntax | `_Noreturn` | `[[noreturn]]` |
| Attribute syntax | No | Yes |

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Token mapping | `src/lexer.cpp:148-149` | `_Noreturn`/`noreturn` → `KW_STATIC` |
| Storage class consumption | `src/parser.cpp:108-110` | `match(KW_STATIC)` appends `static ` to type string |

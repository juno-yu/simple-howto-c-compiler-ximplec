# Lesson 1005: _Atomic (C11)

## Status: ⚠️ Partial | Standard: C11 | Effort: Hard

## Objective

Recognize `_Atomic` as a type qualifier. Real atomic operations are **not** implemented.

## Syntax

```c
_Atomic int counter = 0;
```

## How It Works

`_Atomic` is a recognised keyword (`src/lexer.cpp:139`) and is consumed by `parse_type_specifier` like any other qualifier:

```cpp
// src/parser.cpp:120-121
} else if (match(TokenType::KW_ATOMIC)) {
    result += "_Atomic ";
}
```

The qualifier string is concatenated onto the variable's type name. There is no `lock`-prefixed instruction emission, no `<stdatomic.h>` shim, and no thread-safe codegen. The example in `1005-c11-atomic/src/example.c` reduces to a normal `int` increment.

```cpp
// src/parser.cpp:81-82 — _Atomic also counts as a type specifier
check(TokenType::KW_THREAD_LOCAL) ||
check(TokenType::KW_ATOMIC)) {
    return true;
}
```

## What Works

| Feature | Status |
|---------|--------|
| `_Atomic int x;` accepted | ✅ |
| `_Atomic` as type qualifier | ✅ |
| `atomic_*` functions | ❌ No `<stdatomic.h>` shim |
| Lock-prefixed instructions | ❌ |
| Memory ordering | ❌ |

## Memory Ordering (intended)

| Order | Description |
|-------|-------------|
| `memory_order_relaxed` | No ordering guarantees |
| `memory_order_consume` | Dependencies preserved |
| `memory_order_acquire` | Reads not reordered before |
| `memory_order_release` | Writes not reordered after |
| `memory_order_seq_cst` | Sequential consistency (default) |

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Lexer | `src/lexer.cpp:139` | `_Atomic` → `KW_ATOMIC` |
| Type-specifier recognition | `src/parser.cpp:81-82` | Includes `KW_ATOMIC` |
| Qualifier string | `src/parser.cpp:120-121` | Appends `_Atomic ` to type |

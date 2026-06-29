# Lesson 1009: <stdatomic.h> (C11)

## Status: ⚠️ Skeleton | Standard: C11 | Effort: Medium

## Objective

Standard atomic types and operations.

## How It Works

simplecc ships **no** `<stdatomic.h>` header in `lib/`. The bundled example in `1009-c11-stdatomic/src/example.c` is a stub that returns `42`. The `_Atomic` qualifier (lesson 1005) is recognised but produces no lock-prefixed code.

## Types (aspirational)

```c
atomic_bool
atomic_char
atomic_int
atomic_long
atomic_size_t
atomic_uintptr_t
atomic_ptrdiff_t
```

## Operations (aspirational)

| Function | Description |
|----------|-------------|
| `atomic_init(obj, value)` | Initialize atomic |
| `atomic_store(obj, value)` | Store value |
| `atomic_load(obj)` | Load value |
| `atomic_exchange(obj, value)` | Swap values |
| `atomic_compare_exchange_weak(obj, expected, desired)` | CAS (weak) |
| `atomic_compare_exchange_strong(obj, expected, desired)` | CAS (strong) |
| `atomic_fetch_add(obj, arg)` | Add and return old |
| `atomic_fetch_sub(obj, arg)` | Subtract and return old |
| `atomic_fetch_or(obj, arg)` | Bitwise OR |
| `atomic_fetch_and(obj, arg)` | Bitwise AND |
| `atomic_fetch_xor(obj, arg)` | Bitwise XOR |

## Source Code References

| Component | File | Status |
|-----------|------|--------|
| `<stdatomic.h>` | `lib/` | ❌ Not present |
| `_Atomic` qualifier | `src/parser.cpp:120-121` | Appended to type string only |

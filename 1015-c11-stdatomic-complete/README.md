# Lesson 1015: Header `<stdatomic.h>` Complete (C11)

## Status: ⚠️ Skeleton | Standard: C11 | Effort: Medium

## Objective

Complete atomic types and operations header.

## How It Works

simplecc ships **no** `<stdatomic.h>` in `lib/`. The bundled example in `1015-c11-stdatomic-complete/src/example.c` is a stub that declares `_Atomic int counter = 0;` (which the parser accepts) and returns the counter unchanged. There is no `atomic_flag`, no `ATOMIC_*_INIT` macro, no `atomic_thread_fence`, and no lock-prefixed codegen.

## Includes (aspirational)

- `atomic_bool`, `atomic_int`, `atomic_long`, etc.
- `ATOMIC_*_INIT` macros
- `atomic_flag` (lock-free boolean)
- `atomic_init` function
- `atomic_thread_fence`, `atomic_signal_fence`

## Source Code References

| Component | File | Status |
|-----------|------|--------|
| `<stdatomic.h>` | `lib/` | ❌ Not present |
| `_Atomic` qualifier | `src/parser.cpp:120-121` | Appended to type string only |

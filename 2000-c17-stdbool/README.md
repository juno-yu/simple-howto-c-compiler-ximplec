# Lesson 2000: stdbool.h (C17)

## Status: ✅ Complete | Standard: C17 | Effort: Trivial

## Objective

Provide `bool`, `true`, `false` macros via header.

## How It Works

`bool` is a C23 keyword in this compiler (see `src/lexer.cpp:114` and lesson 3000), so the C17 header shim only needs to define the macros:

```c
// lib/stdbool.h
#define true 1
#define false 0
#define __bool_true_false_are_defined 1
```

`true` and `false` are also pre-defined as built-in macros by the preprocessor at startup:

```cpp
// src/preprocessor.cpp:30-31
macros_["true"]  = Macro("true",  "1");
macros_["false"] = Macro("false", "0");
```

The example in `2000-c17-stdbool/src/example.c` does `#include <stdbool.h>` and then writes `bool flag = true;`. The include brings in the macros; the keyword `bool` works either way.

## C17 Notes

- No changes from C11.
- `<stdbool.h>` provides: `bool`, `true`, `false`.
- In simplecc, `bool` is a **keyword** (not a macro), so the header is only needed for `true`/`false` (and the convenience macro `__bool_true_false_are_defined`).

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Header shim | `lib/stdbool.h` | `true`/`false` macros |
| Built-in macros | `src/preprocessor.cpp:30-31` | Also defined unconditionally |
| `bool` keyword | `src/lexer.cpp:114` | `bool` → `KW_BOOL` |

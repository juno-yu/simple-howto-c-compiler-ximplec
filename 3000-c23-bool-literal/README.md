# Lesson 3000: bool, true, false (C23)

## Status: ✅ Complete (with caveat) | Standard: C23 | Effort: Easy

## Objective

Native `bool`, `true`, `false` keywords (C23).

## How It Works

`bool` is a real keyword:

```cpp
// src/lexer.cpp:114
{"bool", TokenType::KW_BOOL},
```

`true` and `false`, however, are **preprocessor macros** (not keywords):

```cpp
// src/preprocessor.cpp:30-31
macros_["true"]  = Macro("true",  "1");
macros_["false"] = Macro("false", "0");
```

So at the token level only `bool` is a keyword; `true`/`false` arrive in the token stream as the integer literal `1` / `0` after preprocessing. This is enough for the C23 use case but means `true` is not a `BoolLiteralNode` in the AST.

The codegen does treat `bool` as a 1-byte type:

```cpp
// src/codegen.cpp:1127-1128
} else if (node.type_name == "bool") {
    emit("mov $1, %rax");
```

```cpp
// src/codegen.cpp:2069
if (type == "bool" || type == "const bool") return 1;
```

## Changes from C11

```c
// C11: requires <stdbool.h>
#include <stdbool.h>

// C23: native keyword
bool flag = true;
```

## Example

```c
int main() {
    bool t = true;     // preprocessor rewrites to: bool t = 1;
    bool f = false;    // preprocessor rewrites to: bool f = 0;
    return t ? 1 : 0;  // returns 1
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|---|
| `bool` keyword | `src/lexer.cpp:114` | `bool` → `KW_BOOL` |
| `true`/`false` macros | `src/preprocessor.cpp:30-31` | Defined at startup |
| `bool` size | `src/codegen.cpp:2069` | 1 byte |
| Sizeof of `bool` | `src/codegen.cpp:1127-1128` | Emits `mov $1, %rax` |

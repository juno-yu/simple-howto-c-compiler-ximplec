# Lesson 1010: _Thread_local (C11)

## Status: ✅ Complete (parser-level) | Standard: C11 | Effort: Medium

## Objective

Recognise `_Thread_local` as a storage-class qualifier.

## Syntax

```c
_Thread_local int counter = 0;  // each thread has its own copy
__thread int tls_var;           // GCC extension
```

## How It Works

`_Thread_local` is a real keyword (`src/lexer.cpp:138`) and is consumed inside `parse_type_specifier`. The qualifier is recorded as part of the type string but does not switch the codegen to a TLS segment:

```cpp
// src/lexer.cpp:138
{"_Thread_local", TokenType::KW_THREAD_LOCAL},
```

```cpp
// src/parser.cpp:118-119
} else if (match(TokenType::KW_THREAD_LOCAL)) {
    result += "_Thread_local ";
}
```

```cpp
// src/parser.cpp:81 — counted as a type specifier start
check(TokenType::KW_THREAD_LOCAL) ||
```

The example in `1010-c11-thread-local/src/example.c` declares `_Thread_local int tls_var = 42;` and returns it from `main`. Since only `main` runs, the value is the regular local and the program returns `42`. There is no TLS codegen path, no `__thread` mapping, and no `.tdata`/`.tbss` emission.

## What Works

| Feature | Status |
|---------|--------|
| `_Thread_local int x;` parses | ✅ |
| Single-threaded use | ✅ |
| `__thread` extension | ❌ Not recognised |
| Actual TLS segment storage | ❌ Codegen treats it as a normal local |

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Token recognition | `src/lexer.cpp:138` | `_Thread_local` → `KW_THREAD_LOCAL` |
| Type-specifier recognition | `src/parser.cpp:81` | Listed in `is_type_specifier` |
| Qualifier string | `src/parser.cpp:118-119` | Appended to type |

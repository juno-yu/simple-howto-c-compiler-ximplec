# Lesson 2002: stdnoreturn.h (C17)

## Status: 📋 Planned | Standard: C17 | Effort: Trivial

## Objective

Provide `noreturn` macro.

## C17 Notes

- No changes from C11
- `<stdnoreturn.h>` provides: `noreturn`
- Maps to `_Noreturn`

## Implementation

- Header defines: `#define noreturn _Noreturn`

## Processing Flow

```mermaid
flowchart TD
    A["noreturn void func()"] --> B[Preprocessor expands to _Noreturn]
    B --> C[Parse function declaration]
    C --> D[Mark function with noreturn attribute]
    D --> E[Function body analysis]
    E --> F{All paths return?}
    F -->|No - correct| G[Accept: function never returns]
    F -->|Yes - warning| H[Warning: noreturn function returns]
    G --> I[Code generation with noreturn hint]
```

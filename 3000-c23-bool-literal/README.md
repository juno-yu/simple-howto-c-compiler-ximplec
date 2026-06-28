# Lesson 3000: bool, true, false (C23)

## Status: ✅ Complete | Standard: C23 | Effort: Easy

## Objective

Native `bool`, `true`, `false` keywords.

## Changes from C11

```c
// C11: requires <stdbool.h>
#include <stdbool.h>

// C23: native keywords
bool flag = true;
```

## Implementation Checklist

- [ ] Add `bool`, `true`, `false` as keywords
- [ ] `_Bool` still available for compatibility
- [ ] Test: `bool x = true; return x;` → 1

## Flow Diagram

```mermaid
flowchart TD
    A[Source: true/false/nullptr] --> B[Lexer]
    B --> C{Token Type?}
    C -->|true| D[Token::TRUE]
    C -->|false| E[Token::FALSE]
    C -->|nullptr| F[Token::NULLPTR]
    D --> G[Parser]
    E --> G
    F --> G
    G --> H[AST: BoolLiteral / NullptrLiteral]
    H --> I[Codegen]
    I --> J{x86-64 Assembly}
    D --> K["mov $1, %rax"]
    E --> L["mov $0, %rax"]
    F --> M["mov $0, %rax"]
```

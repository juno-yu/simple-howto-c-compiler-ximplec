# Lesson 3009: nullptr (C23)

## Status: ✅ Complete | Standard: C23 | Effort: Easy

## Objective

Null pointer constant keyword.

## Syntax

```c
int *p = nullptr;      // instead of NULL or 0
if (p == nullptr) { }  // explicit null check
```

## C23 vs C11

| Feature | C11 | C23 |
|---------|-----|-----|
| Null pointer | `NULL` (macro) | `nullptr` (keyword) |
| Type safety | Weak | Strong |
| Usable in `_Generic` | No | Yes |

## Implementation Checklist

- [ ] Add `nullptr` keyword
- [ ] `nullptr` has type `void*` (or dedicated null pointer type)
- [ ] `nullptr == ptr` comparisons
- [ ] `if (ptr)` still works
- [ ] Test: `int *p = nullptr; return p == nullptr;` → 1

## Flow Diagram

```mermaid
flowchart TD
    A[Source: int *p = nullptr] --> B[Lexer]
    B --> C{nullptr keyword?}
    C -->|Yes| D[Token: NULLPTR]
    C -->|No| E[Normal token]
    D --> F[Parser]
    E --> F
    F --> G{Assignment to pointer?}
    G -->|Yes| H[Type check: pointer type]
    G -->|No| I[Error: nullptr not for non-pointers]
    H --> J[Generate null pointer constant]
    J --> K[AST: NullptrExpr]
    K --> L[Codegen]
    L --> M[Assembly: mov $0, %reg]
    F --> N{Comparison with nullptr?}
    N -->|Yes| O[Generate null check]
    N -->|No| P[Normal comparison]
    O --> Q[Assembly: test %reg, %reg]
    P --> Q
```

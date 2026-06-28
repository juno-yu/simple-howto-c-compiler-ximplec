# Lesson 3011: auto Arrays (C23)

## Status: 📋 Planned | Standard: C23 | Effort: Easy

## Objective

Infer array size from initializer.

## Syntax

```c
auto arr[] = {1, 2, 3};       // int[3]
auto arr2[] = {1.0, 2.0};     // double[2]
```

## Implementation Checklist

- [ ] Parse `auto arr[] = {init-list}`
- [ ] Infer element type from initializer
- [ ] Infer size from number of elements
- [ ] Test: `auto a[] = {1,2,3}; return sizeof(a);` → 12

## Flow Diagram

```mermaid
flowchart TD
    A[Source: auto arr[] = 1,2,3] --> B[Lexer]
    B --> C{auto with array?}
    C -->|Yes| D[Parse auto array declaration]
    C -->|No| E[Normal declaration]
    D --> F[Parse initializer list]
    F --> G[Count elements: 3]
    G --> H[Infer element type: int]
    H --> I[Infer size: 3]
    I --> J[Generate array type: int[3]]
    J --> K[AST: AutoArrayDecl]
    K --> L[Codegen]
    L --> M[Assembly: allocate 12 bytes]
```

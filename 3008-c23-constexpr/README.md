# Lesson 3008: constexpr (C23)

## Status: ✅ Complete | Standard: C23 | Effort: Medium

## Objective

Compile-time constant expressions.

## Syntax

```c
constexpr int size = 10;
constexpr int arr[size] = {0};
constexpr int square(int x) { return x * x; }
```

## Implementation Checklist

- [ ] Parse `constexpr` keyword
- [ ] Validate initializer is constant expression
- [ ] Use in array sizes and switch cases
- [ ] Support constexpr functions
- [ ] Test: `constexpr int x = 42;` → compile-time constant

## Flow Diagram

```mermaid
flowchart TD
    A[Source: constexpr int x = 42] --> B[Lexer]
    B --> C{constexpr keyword?}
    C -->|Yes| D[Parse constexpr declaration]
    C -->|No| E[Normal declaration]
    D --> F[Validate initializer]
    F --> G{Is constant expression?}
    G -->|Yes| H[Mark as compile-time constant]
    G -->|No| I[Error: not constant]
    H --> J[Store in symbol table]
    J --> K[Use in array sizes]
    J --> L[Use in switch cases]
    J --> M[Evaluate at compile time]
    K --> N[Parser]
    L --> N
    M --> N
    N --> O[AST: ConstexprDecl]
    O --> P[Codegen]
    P --> Q[Assembly with constants]
```

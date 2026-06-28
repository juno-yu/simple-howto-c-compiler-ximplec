# Lesson 3010: constexpr Functions (C23)

## Status: 📋 Planned | Standard: C23 | Effort: Medium

## Objective

Functions evaluable at compile time.

## Syntax

```c
constexpr int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

constexpr int fact10 = factorial(10);  // computed at compile time
```

## Rules

- Body must be single return statement (C23 initial)
- All expressions must be constant expressions
- No static/local variables
- No pointer arguments (C23 relaxation may come later)

## Implementation Checklist

- [ ] Parse `constexpr` on functions
- [ ] Validate function body is constant expression
- [ ] Evaluate at compile time when called with constants
- [ ] Fall back to runtime if args not constant
- [ ] Test: `constexpr int f(int x) { return x * 2; }`

## Flow Diagram

```mermaid
flowchart TD
    A[Source: constexpr int f - int x] --> B[Lexer]
    B --> C{constexpr on function?}
    C -->|Yes| D[Parse as constexpr function]
    C -->|No| E[Parse as normal function]
    D --> F[Validate body]
    F --> G{Body is constant expression?}
    G -->|Yes| H[Mark as constexpr]
    G -->|No| I[Error: not constexpr]
    H --> J[Store in symbol table]
    J --> K{Called with constants?}
    K -->|Yes| L[Evaluate at compile time]
    K -->|No| M[Generate runtime code]
    L --> N[AST: ConstexprFuncDecl]
    M --> N
    N --> O[Codegen]
    O --> P[Assembly: compile-time or runtime]
```

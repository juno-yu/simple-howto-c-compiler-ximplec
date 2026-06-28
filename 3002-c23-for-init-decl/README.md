# Lesson 3002: For-Loop Init Declarations (C23)

## Status: 📋 Planned | Standard: C23 | Effort: Easy

## Objective

Allow variable declaration in for-loop initializer without C99 restriction.

## Syntax

```c
// C99: requires type
for (int i = 0; i < 10; i++) { }

// C23: allows any declaration
for (auto i = 0; i < 10; i++) { }
for (int i = 0, j = 10; i < j; i++, j--) { }
```

## Changes from C99

- C99: only `for (type var = init; ...; ...)`
- C23: allows `for (decl; ...; ...)` where decl is any declaration
- Scoped to for-loop body only

## Implementation Checklist

- [ ] Allow any declaration in for-loop init
- [ ] Variable scope limited to for-loop
- [ ] Support multiple declarations: `for (int i = 0, j = 10; ...)`
- [ ] Support `auto` in for-loop init
- [ ] Test: `for (auto i = 0; i < 5; i++) { }`
- [ ] Test: variable not accessible after loop

## Flow Diagram

```mermaid
flowchart TD
    A[Source: for int i = 0;] --> B[Lexer]
    B --> C[Parser]
    C --> D{For Statement?}
    D --> E[Parse Init Declaration]
    E --> F[Parse Condition]
    F --> G[Parse Update]
    G --> H[Parse Body]
    H --> I[AST: ForStmt with DeclInit]
    I --> J[Codegen]
    J --> K[Init Code]
    K --> L[Loop Label]
    L --> M{Condition}
    M -->|true| N[Body + Update]
    N --> L
    M -->|false| O[Exit Loop]
```

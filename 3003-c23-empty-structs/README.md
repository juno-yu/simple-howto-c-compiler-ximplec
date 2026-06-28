# Lesson 3003: Empty Structures (C23)

## Status: 📋 Planned | Standard: C23 | Effort: Easy

## Objective

Allow empty struct and union declarations.

## Syntax

```c
struct empty { };  // C23: valid, size = 1
union empty { };   // C23: valid, size = 1
```

## C11 vs C23

```c
// C11: empty struct is a constraint violation
struct empty { };  // error: invalid

// C23: empty struct is allowed
struct empty { };  // valid, sizeof(struct empty) == 1
```

## Implementation Checklist

- [ ] Allow empty struct/union declarations
- [ ] Set size to 1 byte (not 0)
- [ ] No members to access
- [ ] Allow array of empty structs
- [ ] Test: `struct empty { }; struct empty e; return sizeof(e);` → 1

## Flow Diagram

```mermaid
flowchart TD
    A[Source: struct empty {}] --> B[Lexer]
    B --> C[Parser]
    C --> D{Struct Declaration?}
    D --> E[Parse Members]
    E --> F{Members Empty?}
    F -->|Yes| G[Allow Empty Struct]
    F -->|No| H[Normal Struct Processing]
    G --> I[Set Size = 1]
    I --> J[AST: StructDecl with 0 members]
    H --> J
    J --> K[Codegen]
    K --> L[Type Size Calculation]
    L --> M{x86-64 Layout}
    G --> N["sizeof = 1 (not 0)"]
```

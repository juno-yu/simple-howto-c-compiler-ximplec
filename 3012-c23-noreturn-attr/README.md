# Lesson 3012: [[noreturn]] Attribute (C23)

## Status: 📋 Planned | Standard: C23 | Effort: Easy

## Objective

Replace `_Noreturn` with attribute syntax.

## Syntax

```c
[[noreturn]] void abort(void);
[[noreturn]] void exit(int);
```

## Changes from C11

- C11: `_Noreturn` keyword
- C23: `[[noreturn]]` attribute (preferred)
- Both should be supported for compatibility

## Implementation Checklist

- [ ] Parse `[[noreturn]]` attribute
- [ ] Apply same semantics as `_Noreturn`
- [ ] Support both syntaxes
- [ ] Test: `[[noreturn]] void die(void) { exit(1); }`

## Flow Diagram

```mermaid
flowchart TD
    A[Source: [[noreturn]] void f] --> B[Lexer]
    B --> C{Double bracket?}
    C -->|Yes| D[Parse attribute]
    C -->|No| E[Normal token]
    D --> F{Attribute name: noreturn?}
    F -->|Yes| G[Set NORETURN flag]
    F -->|No| H[Ignore unknown attribute]
    G --> I[Attach to function declaration]
    H --> I
    I --> J[Parser]
    J --> K[AST: FuncDecl with noreturn]
    K --> L[Codegen]
    L --> M[Assembly: no return path]
```

# Lesson 0011: Forward Declarations

## Status: 📋 Planned | Phase: Quick Wins | Effort: Easy (2-3h)

## Objective

Support function declarations without body (prototypes).

## How Forward Declarations Work

```mermaid
flowchart TD
    A[Source Code] --> B[Lexer]
    B --> C[Parser]
    C --> D{Function has body?}
    D -->|Yes| E[Generate Function Code]
    D -->|No| F[Store as Declaration]
    F --> G[Symbol Table]
    G --> H[Validate at Definition]
    E --> I[Assembly Output]
```

## Implementation Checklist

- [ ] Parse function declarations without body
- [ ] Store declarations in symbol table
- [ ] Validate declaration matches definition
- [ ] Support `extern` variable declarations
- [ ] Test: forward declare and use function before definition

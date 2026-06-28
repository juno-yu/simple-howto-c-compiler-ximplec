# Lesson 0007: Ternary Operator

## Status: 📋 Planned | Phase: Quick Wins | Effort: Easy (3-4h)

## Objective

Implement `cond ? then_expr : else_expr`.

## Implementation Checklist

- [ ] Add `ConditionalExprNode` to AST: `{ condition, then_expr, else_expr }`
- [ ] Parse `? :` in `parse_assignment()` after `||`
- [ ] Codegen: short-circuit evaluation with labels
- [ ] Test: `int max = (a > b) ? a : b;`
- [ ] Test: nested ternary

## Generated Assembly Pattern

```asm
    # condition
    mov -8(%rbp), %rax
    cmp $0, %rax
    je .Lelse_0
    # then_expr
    mov -8(%rbp), %rax
    jmp .Lend_0
.Lelse_0:
    # else_expr
    mov -16(%rbp), %rax
.Lend_0:
```

## Implementation Flow

```mermaid
flowchart TD
    A[Source Code] --> B[Lexer Tokenization]
    B --> C[Parser: parse_assignment]
    C --> D{Token Type?}
    D -->|QUESTION| E[Parse condition]
    E --> F[Parse then_expr]
    F --> G[Parse : else_expr]
    G --> H[TernaryExprNode]
    H --> I[Codegen: visitTernaryExprNode]
    I --> J[Evaluate condition]
    J --> K[cmp $0, %rax]
    K --> L{Condition true?}
    L -->|Yes| M[Evaluate then_expr]
    M --> N[jmp end_label]
    L -->|No| O[Evaluate else_expr]
    N --> P[end_label]
    O --> P

    style H fill:#f9f,stroke:#333,stroke-width:2px
    style I fill:#bbf,stroke:#333,stroke-width:2px
```

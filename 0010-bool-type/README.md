# Lesson 0010: _Bool / bool Type

## Status: 📋 Planned | Phase: Quick Wins | Effort: Easy (1-2h)

## Objective

Implement `bool` type (non-zero → 1, zero → 0).

## Implementation Checklist

- [ ] Add `bool` keyword (or `#define bool _Bool`)
- [ ] Add `_Bool` type (size = 1 byte)
- [ ] Codegen: non-zero → 1, zero → 0
- [ ] `!expr` codegen
- [ ] Test: `bool b = 42; return b;` → 1
- [ ] Test: `return !0;` → 1, `return !1;` → 0

## Implementation Flow

```mermaid
flowchart TD
    A[Source Code] --> B[Lexer: KW_BOOL token]
    B --> C[Parser: recognize bool type]
    C --> D[VarDeclNode with type=bool]
    D --> E[Codegen: visitVarDeclNode]
    E --> F[get_type_size returns 1]
    F --> G[Codegen: visitIntegerLiteralNode]
    G --> H[emit: mov value, %rax]
    H --> I[Store to stack]

    J[!expr] --> K[Codegen: visitUnaryExprNode]
    K --> L[cmp $0, %rax]
    L --> M[sete %al]
    M --> N[movzbq %al, %rax]

    style D fill:#f9f,stroke:#333,stroke-width:2px
    style K fill:#bbf,stroke:#333,stroke-width:2px
```

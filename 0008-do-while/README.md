# Lesson 0008: Do-While Loops

## Status: 📋 Planned | Phase: Quick Wins | Effort: Easy (3-4h)

## Objective

Implement `do { ... } while (cond);` with proper break/continue support.

## Implementation Checklist

- [ ] Add `KW_DO` token to lexer
- [ ] Parse `do stmt while (expr);`
- [ ] Add `DoWhileStmtNode` to AST
- [ ] Codegen: body first, then condition check
- [ ] Test: do-while executes at least once
- [ ] Test: break exits loop, continue jumps to condition

## Implementation Flow

```mermaid
flowchart TD
    A[Source Code] --> B[Lexer: KW_DO token]
    B --> C[Parser: parse_statement]
    C --> D[Match 'do']
    D --> E[Parse body]
    E --> F[Match 'while']
    F --> G[Parse condition]
    G --> H[Match ';']
    H --> I[DoWhileStmtNode]
    I --> J[Codegen: visitDoWhileStmtNode]
    J --> K[do_while_start label]
    K --> L[Emit body code]
    L --> M[Evaluate condition]
    M --> N[cmp $0, %rax]
    N --> O[jne do_while_start]
    O --> P[do_while_end label]

    style I fill:#f9f,stroke:#333,stroke-width:2px
    style J fill:#bbf,stroke:#333,stroke-width:2px
```

## Implementation Details

### Source Code References
| Component | File | Lines | Description |
|-----------|------|-------|-------------|
| Token Definition | src/token.h | 30 | `KW_DO` token type |
| AST Node | src/ast.h | 35, 349-355 | `DO_WHILE_STMT` node type and `DoWhileStmtNode` struct |
| Parser | src/parser.cpp | 661, 764-779 | `parse_do_while_stmt()` function |
| Code Generator | src/codegen.cpp | 164-165, 564-587 | `visit(DoWhileStmtNode&)` implementation |
| Visitor Interface | src/ast.h | 149 | `visit(DoWhileStmtNode&)` pure virtual method |

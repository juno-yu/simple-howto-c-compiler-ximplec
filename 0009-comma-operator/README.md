# Lesson 0009: Comma Operator

## Status: ✅ Complete | Phase: Quick Wins | Effort: Easy (1-2h)

## Objective

Implement `expr1, expr2` for sequential evaluation.

## Implementation Checklist

- [x] Add `CommaExprNode` to AST: `{ left, right }`.
- [x] Parse comma in `parse_expression()` (the lowest-precedence level).
- [x] Codegen: evaluate left, discard, evaluate right (result in `%rax`).
- [x] Test: `int a = (1, 2, 3);` → `a = 3`.
- [x] Test: `for (i = 0, j = 10; i < j; i++, j--) {}` — comma appears
      inside the `for` header where allowed.

## Implementation Flow

```mermaid
flowchart TD
    A[Source Code] --> B[Lexer Tokenization]
    B --> C[Parser: parse_expression]
    C --> D{COMMA token?}
    D -->|No| E[Return left expr]
    D -->|Yes| F[Evaluate left expression]
    F --> G[Consume comma]
    G --> H[Parse right expression]
    H --> I[CommaExprNode]
    I --> J[Codegen: visitCommaExprNode]
    J --> K[Dispatch left - discard result]
    K --> L[Dispatch right - result stays in %rax]

    style I fill:#f9f,stroke:#333,stroke-width:2px
    style J fill:#bbf,stroke:#333,stroke-width:2px
```

## Core Implementation Snippets

The parser's lowest-precedence level loops on `,`, building a
left-leaning chain of `CommaExprNode`s.

```cpp
// src/parser.cpp:1418
ASTPtr Parser::parse_expression() {
    auto left = parse_assignment();

    while (match(TokenType::COMMA)) {
        auto comma = std::make_unique<CommaExprNode>(left->line, left->column);
        comma->left  = std::move(left);
        comma->right = parse_assignment();
        left = std::move(comma);
    }
    return std::move(left);
}
```

The codegen is intentionally trivial: evaluate both sides, let the right
overwrite the left in `%rax`.

```cpp
// src/codegen.cpp:1115
void CodeGenerator::visit(CommaExprNode& node) {
    dispatch(node.left.get());
    dispatch(node.right.get());
}
```

## Implementation Details

### Source Code References

| Component | File | Lines | Description |
|-----------|------|-------|-------------|
| `COMMA_EXPR` node type | src/ast.h | 46 | Enum value |
| `CommaExprNode` struct | src/ast.h | 454-461 | `left` + `right` |
| `visit(CommaExprNode&)` declaration | src/ast.h | 171 | Pure virtual in `ASTVisitor` |
| `parse_expression()` | src/parser.cpp | 1418-1431 | Comma loop wraps the assignment result |
| `visit(CommaExprNode&)` impl | src/codegen.cpp | 1115-1118 | Evaluate both; right remains in `%rax` |
| `dispatch` case for `COMMA_EXPR` | src/codegen.cpp | 216-218 | NodeType → visit switch |
| Type inference | src/codegen.cpp | 2367-2370 | Result type = type of right-hand operand |

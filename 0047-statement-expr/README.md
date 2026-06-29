# Lesson 0047: Statement Expressions (GCC Extension)

## Status: ✅ Complete | Phase: System Integration | Effort: Medium

## Objective

Support the GCC statement-expression syntax `({ stmt; expr; })` — a
parenthesised compound statement that is itself an expression whose value is
the value of the last expression-statement inside the braces.

## Syntax

```c
int x = ({
    int a = 5;
    int b = 10;
    a + b;        // last expression becomes the value
});
                // x is now 15
```

## Semantics

- The braces introduce a new block scope; local declarations inside are valid.
- The value of the statement expression is the value of the last expression
  statement. The result is left in `%rax` by the codegen, just like any
  other expression.
- Statement expressions can be nested and can appear wherever a normal
  expression can (RHS of `=`, function argument, return value, etc.).

## AST Node

The `StmtExprNode` AST node (`src/ast.h:525-530`) wraps a `BlockNode`:

```cpp
// src/ast.h:525
struct StmtExprNode : ASTNode {
    ASTPtr body; // BlockNode containing statements, last one is the value

    StmtExprNode(int l, int c) : ASTNode(NodeType::STMT_EXPR, l, c) {}
    void accept(ASTVisitor& visitor);
};
```

## Parsing

When the parser sees `(` followed by `{` it treats the construct as a
statement expression:

```cpp
// src/parser.cpp:1988
if (match(TokenType::LPAREN)) {
    if (check(TokenType::LBRACE)) {
        int line = tokens_[pos_ - 1].line;
        int col = tokens_[pos_ - 1].column;
        auto block = parse_block();
        expect(TokenType::RPAREN);
        return std::make_unique<StmtExprNode>(line, col);
    }
    // ... regular parenthesised expression
}
```

## Code Generation

The codegen simply walks the inner block, statement by statement. The final
expression's value is left in `%rax` by the last `visit(ExprStmtNode)` /
`visit(BinaryExprNode)` call:

```cpp
// src/codegen.cpp:1483
void CodeGenerator::visit(StmtExprNode& node) {
    if (node.body) {
        auto& block = static_cast<BlockNode&>(*node.body);
        for (auto& stmt : block.statements) {
            dispatch(stmt.get());
        }
    }
}
```

## Limitations

- The "value" of the statement expression is whatever the last statement
  happens to leave in `%rax` (or `%xmm0` for a float). The compiler does not
  enforce that the last statement is actually an expression — if it is a
  declaration or an empty block, the value is undefined.
- `break`/`continue` cannot legally target a label outside the statement
  expression; the compiler does not track or enforce this.
- No diagnostics are produced if the last statement is non-value-yielding.

## Example

```c
int main() {
    int x = ({
        int a = 5;
        int b = 10;
        a + b;
    });
    return x;       // returns 15
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `StmtExprNode` | `src/ast.h:525-530` | AST node holding a `BlockNode` body |
| Parser | `src/parser.cpp:1988-1995` | `parse_primary` detects `({` and builds the node |
| Codegen | `src/codegen.cpp:1483-1490` | Walks the inner block in order |
| `visit` dispatch | `src/codegen.cpp:241` | Hooked into the visitor switch |

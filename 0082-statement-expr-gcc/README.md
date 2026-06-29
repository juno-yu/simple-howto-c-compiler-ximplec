# Lesson 0082: Statement Expressions `({...})` (GCC Extension)

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Medium

## Objective

Support the GCC statement expression syntax: a parenthesised
compound statement that yields a value (the value of its last
expression statement). The result can be used wherever an
expression is expected — including as the right-hand side of an
assignment, a function argument, or a return value.

## Syntax

```c
int x = ({
    int a = 5;
    int b = 10;
    a + b;        // last expression becomes the value
});                // x is initialized to 15
```

## Semantics

- Inside `({...})`, declarations are allowed (like a block).
- The value of the statement expression is the value of the last
  expression statement. If the last statement is not an expression
  statement, the value is undefined.
- Multiple `({...})` can be nested.
- Jumps into a statement expression (e.g. `goto` from outside) are
  not allowed.

## AST Node

```cpp
// src/ast.h:525
struct StmtExprNode : ASTNode {
    ASTPtr body; // BlockNode containing statements, last one is the value

    StmtExprNode(int l, int c) : ASTNode(NodeType::STMT_EXPR, l, c) {}
    void accept(ASTVisitor& visitor);
};
```

## Parsing

When the parser sees `(` followed by `{` in expression position, it
treats the construct as a statement expression:

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

The codegen walks the inner block in order. The final expression's
value is left in `%rax` by the last dispatched statement — exactly
like a normal expression:

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

## What Works

- `({ stmt; expr; })` parses and emits correctly.
- Nested statement expressions are supported.
- Statement expressions can appear in any expression position
  (RHS of `=`, function argument, return value, etc.).
- Local declarations inside the block are properly scoped.

## Limitations

- **No value-type checking** between the last expression and the
  surrounding context.
- **`break`/`continue` cannot target labels outside the statement
  expression** (we don't track this).
- **Type of empty statement expression** is treated as `int` with
  undefined value.

## Example

```c
int main() {
    int x = ({
        int a = 5;
        int b = 10;
        a + b;     // value of the ({...}) is 15
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
| Visitor dispatch | `src/codegen.cpp:241` | Hooked into the visitor switch |

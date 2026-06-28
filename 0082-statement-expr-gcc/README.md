# Lesson 0082: Statement Expressions `({...})` (GCC Extension)

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Medium

## Objective

Support the GCC statement expression syntax: a parenthesized compound statement that yields a value (the value of its last expression statement). The result can be used wherever an expression is expected — including as the right-hand side of an assignment.

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
- The value of the statement expression is the value of the last expression statement. If the last statement is not an expression statement, the value is `void` (or undefined).
- Multiple `({...})` can be nested.
- Jumps into a statement expression (e.g., `goto` from outside) are not allowed.

## Implementation

The lexer recognizes `(` followed by `{` as the start of a statement expression. The parser recursively parses the block, then expects the closing `)`. The codegen emits the block in-place, leaving the result of the last expression in `%rax` (the x86-64 return register).

A dedicated AST node `StmtExprNode` holds the block and integrates with normal expression-visitor dispatch.

## Limitations

- **No value-type checking** between the last expression and the surrounding context.
- **`break`/`continue` cannot target labels outside the statement expression** (we don't track this).
- **Type of empty statement expression** is treated as `int` with undefined value.

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

| Component | File | Description |
|-----------|------|-------------|
| AST node | `src/ast.h` | `StmtExprNode` declaration |
| Lexer | `src/lexer.cpp` | `({` token sequence detection |
| Parser | `src/parser.cpp` | `parse_primary` handles `({` |
| Codegen | `src/codegen.cpp` | `visitStmtExprNode` emits block in-place |

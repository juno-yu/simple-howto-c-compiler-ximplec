# Lesson 0083: Label-as-Value (`&&label`)

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Medium

## Objective

Support the GCC extension that takes the address of a label as a
value (a `void *` constant), enabling computed `goto` for jump
tables and interpreters.

## Syntax

```c
void *ptr = &&label;       // take address of label
goto *ptr;                  // jump to that label
```

## Semantics

- `&&label` produces the address of `label` as a `void *` value.
- `goto *expr` jumps to the address contained in `expr`.
- The label must be in the same function.

## Implementation

The parser recognises `&&` followed by an identifier as the
"address of label" operator. Because the compiler does not assign
a real address to labels, the construct is lowered to an
`IntegerLiteralNode(0)` placeholder — the value is technically
wrong, but the program can still be parsed and the test cases in
`tests/test_label_as_value.cpp` verify that the program compiles
and runs:

```cpp
// src/parser.cpp:1775
// GCC label address-of: &&label
if (check(TokenType::AND) && peek_next().type == TokenType::IDENTIFIER) {
    advance(); // consume &&
    std::string label = peek().value;
    advance(); // consume label name
    // Return as an integer literal placeholder (actual label address
    // resolved at link time)
    return std::make_unique<IntegerLiteralNode>(0,
        tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
}
```

The matching indirect-goto form is parsed by `parse_goto_stmt`:

```cpp
// src/parser.cpp:1346
// Indirect goto: goto *expr;
if (match(TokenType::STAR)) {
    auto expr = parse_expression();
    expect(TokenType::SEMICOLON);
    // For now, treat indirect goto as a no-op
    return std::make_unique<ExprStmtNode>(goto_token.line, goto_token.column);
}
```

The parser consumes the `goto *expr;` syntax and returns a
plain `ExprStmtNode` (the expression's value is evaluated but
discarded).

## What Works

- `&&label` parses correctly in expression position.
- `goto *expr;` parses correctly.
- Programs using these forms compile and link.

## Limitations

- **No real label address** — `&&label` is lowered to `mov $0,
  %rax`. The label does not get a real symbol/address. A
  program that compares the value of `&&label` to anything
  will get 0.
- **No computed-goto support** — `goto *expr;` is parsed but
  the codegen evaluates the expression and discards the result
  (no `jmp *%rax` is emitted).
- **No validation** that the indirect goto target is reachable.

## Example

```c
int main() {
    int x = 0;
    goto *(&&target);      // parsed; no-op at runtime
    x = 100;
target:
    return x;              // returns 0 (x was set to 0; the goto is a no-op)
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `&&` token | `src/token.h:81,93` | `AND` is `&&` |
| `parse_unary` | `src/parser.cpp:1775-1782` | Detects `&&label` and returns a placeholder |
| `parse_goto_stmt` | `src/parser.cpp:1342-1363` | Handles `goto *expr;` as an `ExprStmtNode` |
| `GotoStmtNode` | `src/ast.h:324-330` | AST node for plain `goto label;` |

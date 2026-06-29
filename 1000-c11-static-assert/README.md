# Lesson 1000: _Static_assert (C11)

## Status: ✅ Complete | Standard: C11 | Effort: Easy

## Objective

Compile-time assertions with optional message.

## Syntax

```c
_Static_assert(constexpr, "message");
_Static_assert(sizeof(int) == 4, "int must be 4 bytes");
```

## How It Works

`_Static_assert` and the C23 spelling `static_assert` are both lexed as the same token (see `src/lexer.cpp:140-141`). The parser consumes the assertion and emits a no-op `ExprStmtNode`:

```cpp
// src/parser.cpp:1366-1381 — parse_static_assert
ASTPtr Parser::parse_static_assert() {
    int line = peek().line;
    int col  = peek().column;
    advance(); // consume _Static_assert
    expect(TokenType::LPAREN);
    auto condition = parse_expression();
    // Skip the message string
    if (match(TokenType::COMMA)) {
        if (check(TokenType::STRING_LITERAL)) advance();
    }
    expect(TokenType::RPAREN);
    expect(TokenType::SEMICOLON);
    // Static assertions are checked at compile time; for now skip them
    // by returning an empty expression statement
    return std::make_unique<ExprStmtNode>(line, col);
}
```

The condition is parsed but its truth value is not evaluated at compile time, so a failing assertion is not caught. The message string is consumed but discarded.

## C11 Spec

- First argument must be an integer constant expression
- Second argument is a string literal (optional in C23)
- Failure produces a diagnostic message

## Limitations

- The condition is **not** evaluated — a `_Static_assert(0, "fail");` is silently accepted. Lesson 1000 covers the syntactic form only.

## Comparison with C23

| Feature | C11 | C23 |
|---------|-----|-----|
| Syntax | `_Static_assert(expr, "msg")` | `static_assert(expr, "msg")` |
| Message required | Yes | No (optional) |

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Token recognition | `src/lexer.cpp:140-141` | `_Static_assert` and `static_assert` mapped to `KW_STATIC_ASSERT` |
| Parser dispatch | `src/parser.cpp:1069-1071` | Routes to `parse_static_assert` |
| `parse_static_assert` | `src/parser.cpp:1366-1381` | Skips expr, message, emits empty stmt |

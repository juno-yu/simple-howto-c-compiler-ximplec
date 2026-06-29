# Lesson 1001: _Generic Selection (C11)

## Status: ⚠️ Partial | Standard: C11 | Effort: Hard

## Objective

Compile-time type-based dispatch without macros.

## Syntax

```c
_Generic(expr, type1: val1, type2: val2, ..., default: val)
```

## How It Works

The parser handles `_Generic` as a unary prefix operator. It walks the association list, parsing the type and result expression for each, but the controlling expression's type is never inspected — the **first** association's value is returned (or the `default` branch's value if it appears first).

```cpp
// src/parser.cpp:1674-1702 — _Generic handling inside parse_unary
if (match(TokenType::KW_GENERIC)) {
    int line = tokens_[pos_ - 1].line;
    int col  = tokens_[pos_ - 1].column;
    expect(TokenType::LPAREN);
    auto control = parse_assignment();
    expect(TokenType::COMMA);

    ASTPtr result = nullptr;
    while (!check(TokenType::RPAREN) && !is_at_end()) {
        if (match(TokenType::KW_DEFAULT)) {
            expect(TokenType::COLON);
            result = parse_assignment();
        } else {
            // Skip type name
            parse_type_specifier();
            expect(TokenType::COLON);
            auto assoc_expr = parse_assignment();
            if (!result) result = std::move(assoc_expr);
        }
        if (!match(TokenType::COMMA)) break;
    }
    expect(TokenType::RPAREN);

    // Simplified: just return the first/default expression
    return result ? std::move(result)
                  : std::make_unique<IntegerLiteralNode>(0, line, col);
}
```

## Limitations

- The control expression is **parsed** but its type is **never inspected** — selection is purely positional (first/default). A `float` argument does not pick the `float:` arm.
- `default` is consumed but the parser also accepts the first non-`default` arm in the same pass, so behavior depends on order in the source.
- All associations are syntactically consumed; unmatched arms do not produce diagnostics.

## Example

```c
int main() {
    int x = _Generic(1, int: 1, char: 2, default: 0);
    return x;  // returns 1
}
```

## Use Cases (aspirational)

- Type-generic macros (tgmath.h)
- Type-safe printf wrappers
- Generic logging functions

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Token recognition | `src/lexer.cpp:142` | `_Generic` → `KW_GENERIC` |
| Parsing | `src/parser.cpp:1674-1702` | Walks associations, returns first expression |

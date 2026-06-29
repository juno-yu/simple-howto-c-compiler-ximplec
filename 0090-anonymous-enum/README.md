# Lesson 0090: Anonymous Enums

## Status: ✅ Complete | Phase: Advanced Types | Effort: Easy

## Objective

Support `enum {A=10, B=20, C=30};` without a name.

## How It Works

The parser detects `enum` followed directly by `{` (no identifier) and adds the constants to `enum_constants_` so they can be resolved wherever they appear.

```cpp
// src/parser.cpp:489-510
if (match(TokenType::KW_ENUM)) {
    // Check for anonymous enum: enum { A, B, C };
    if (check(TokenType::LBRACE)) {
        expect(TokenType::LBRACE);
        long long val = 0;
        while (!check(TokenType::RBRACE) && !is_at_end()) {
            std::string cname = peek().value;
            advance();
            if (match(TokenType::ASSIGN)) {
                auto expr = parse_assignment();
                if (expr && expr->type == NodeType::INTEGER_LITERAL) {
                    val = static_cast<IntegerLiteralNode*>(expr.get())->value;
                }
            }
            enum_constants_[cname] = val;   // ← resolved at parse time
            match(TokenType::COMMA);
            val++;
        }
        expect(TokenType::RBRACE);
        match(TokenType::SEMICOLON);
        return std::make_unique<ExprStmtNode>(...);
    }
    return parse_enum_decl();
}
```

Identifiers later in the token stream are resolved to integer literals at the point of use:

```cpp
// src/parser.cpp:1982-1983
if (enum_constants_.count(tok.value)) {
    return std::make_unique<IntegerLiteralNode>(enum_constants_[tok.value], ...);
}
```

## Status

| Feature | Status |
|---------|--------|
| Named enum `enum Color {RED, GREEN}` | ✅ |
| Anonymous enum `enum {A=10, B=20}` | ✅ Constants added to `enum_constants_` |

## Example

```c
int main() {
    enum {A=10, B=20, C=30};
    return B;  // returns 20
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Anonymous enum branch | `src/parser.cpp:489-510` | Walks `{ ... }` and fills `enum_constants_` |
| Constant resolution | `src/parser.cpp:1982-1983` | Replaces identifier with literal |
| Storage | `src/parser.h:88` | `enum_constants_` map |

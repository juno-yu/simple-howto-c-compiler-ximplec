# Lesson 1007: _Alignas and _Alignof (C11)

## Status: ✅ Complete (parser-level) | Standard: C11 | Effort: Easy

> Folder name `1007-c11-memalign` reflects the original lesson slot; the README documents the alignment specifiers the lesson actually covers. See lesson 1014 for `<stdalign.h>` shim macros.

## Objective

Recognize alignment specifiers and the `_Alignof` operator.

## Syntax

```c
_Alignas(16) char buffer[256];  // aligned to 16 bytes
_Alignof(int)                    // returns alignment requirement of int
```

## How It Works

Both keywords are mapped to dedicated tokens by the lexer:

```cpp
// src/lexer.cpp:143-146
{"_Alignof", TokenType::KW_ALIGNOF},
{"alignof",  TokenType::KW_ALIGNOF},
{"_Alignas", TokenType::KW_ALIGNAS},
{"alignas",  TokenType::KW_ALIGNAS},
```

`_Alignas(N)` is consumed and skipped inside `parse_type_specifier` — no alignment is actually applied to the variable's storage:

```cpp
// src/parser.cpp:122-128
} else if (match(TokenType::KW_ALIGNAS)) {
    // _Alignas(N) - skip the alignment specifier
    if (match(TokenType::LPAREN)) {
        parse_expression();
        expect(TokenType::RPAREN);
    }
}
```

`_Alignof(expr)` is lowered to a `SizeofExprNode` (the codegen reuses the sizeof path):

```cpp
// src/parser.cpp:1653-1672
// _Alignof operator (same as sizeof for alignment purposes)
if (match(TokenType::KW_ALIGNOF)) {
    int line = tokens_[pos_ - 1].line;
    int col  = tokens_[pos_ - 1].column;
    if (match(TokenType::LPAREN)) {
        if (is_type_specifier()) {
            std::string type = parse_type_specifier();
            expect(TokenType::RPAREN);
            return std::make_unique<SizeofExprNode>(type, line, col);
        } else {
            auto expr = parse_expression();
            expect(TokenType::RPAREN);
            return std::make_unique<SizeofExprNode>(std::move(expr), line, col);
        }
    } else {
        auto expr = parse_unary();
        return std::make_unique<SizeofExprNode>(std::move(expr), line, col);
    }
}
```

```cpp
// src/codegen.cpp:1120-1145 — visit(SizeofExprNode)
void CodeGenerator::visit(SizeofExprNode& node) {
    if (node.is_type) {
        if (node.type_name == "int" || node.type_name == "const int") {
            emit("mov $4, %rax");
        } else if (node.type_name == "char" || node.type_name == "const char") {
            emit("mov $1, %rax");
        }
        // ... other known types ...
    }
}
```

`_Alignof(int)` therefore emits the same `mov $4, %rax` as `sizeof(int)`.

## What Works

| Feature | Status |
|---------|--------|
| `_Alignof(int)` returns size of `int` (4) | ✅ |
| `_Alignas(N)` parsed and discarded | ✅ |
| Real alignment-aware allocation | ❌ — `sub $N, %rsp` is not adjusted for alignment |

## Common Alignments (aspirational)

| Type | Alignment |
|------|-----------|
| char | 1 |
| short | 2 |
| int | 4 |
| long | 8 |
| pointer | 8 |
| SIMD types | 16, 32 |

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Token recognition | `src/lexer.cpp:143-146` | `_Alignof`/`alignof`/`_Alignas`/`alignas` |
| `_Alignas` skip | `src/parser.cpp:122-128` | Discards specifier |
| `_Alignof` parse | `src/parser.cpp:1653-1672` | Reuses `SizeofExprNode` |
| Sizeof codegen | `src/codegen.cpp:1120-1145` | Emits literal byte counts |

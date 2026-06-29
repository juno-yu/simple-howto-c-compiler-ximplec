# Lesson 3002: For-Loop Init Declarations (C23)

## Status: ✅ Complete | Standard: C23 | Effort: Easy

## Objective

Allow any declaration in the for-loop init slot.

## How It Works

`parse_for_stmt` already handles both branches — a type specifier (declaration) or an expression. The C23 form falls out for free because the same code path serves C99's `for (int i = 0; ...)` and any other declaration.

```cpp
// src/parser.cpp:1258-1296 — parse_for_stmt
// Init
if (is_type_specifier()) {
    std::string type = parse_type_specifier();
    if (check(TokenType::IDENTIFIER)) {
        advance(); // consume identifier — parse_var_decl expects pos past name
    }
    for_stmt->init = parse_var_decl(type);
} else if (!check(TokenType::SEMICOLON)) {
    for_stmt->init = parse_expression();     // ← also handles `for (i = 0; ...)`
    expect(TokenType::SEMICOLON);
} else {
    advance(); // empty-init
}
```

The bundled example in `3002-c23-for-init-decl/src/example.c` uses `for (i = 0; ...)` (init-from-expression), and returns `5` when `i == 5`.

## Syntax

```c
// C99: requires type
for (int i = 0; i < 10; i++) { }

// C23: any declaration
for (auto i = 0; i < 10; i++) { }
for (int i = 0, j = 10; i < j; i++, j--) { }
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|---|
| `parse_for_stmt` | `src/parser.cpp:1258-1296` | Dispatches on type-specifier vs expression |
| Comma in init/update | `src/parser.cpp:1275, 1289` | Uses `parse_expression()` |

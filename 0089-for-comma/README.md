# Lesson 0089: Comma Expressions in For Loops

## Status: ✅ Complete | Phase: Control Flow | Effort: Easy

## Objective

Support comma expressions in for-loop init and update: `for (i=0, s=0; ...; i++, s+=i)`.

## Implementation

`parse_for_stmt` uses `parse_expression()` (which handles commas) for both the init and update slots.

```cpp
// src/parser.cpp:1258-1296 — parse_for_stmt
// Init
if (is_type_specifier()) {
    std::string type = parse_type_specifier();
    if (check(TokenType::IDENTIFIER)) advance();
    for_stmt->init = parse_var_decl(type);
} else if (!check(TokenType::SEMICOLON)) {
    for_stmt->init = parse_expression();        // ← comma-capable
    expect(TokenType::SEMICOLON);
} else {
    advance(); // empty-init
}

// Update
if (!check(TokenType::RPAREN)) {
    for_stmt->update = parse_expression();    // ← comma-capable
}
```

## What Works

| Pattern | Status |
|---------|--------|
| `for (;;) { }` | ✅ |
| `for (; cond; ) { }` | ✅ |
| `for (; cond; update) { }` | ✅ |
| `for (int i = 0; ...) { }` | ✅ Declaration in init |
| `for (i = 0; i < 3; i++) { }` | ✅ Expression init |
| Comma in init: `for (i=0, s=0; ...)` | ✅ |
| Comma in update: `for (...; i++, s++)` | ✅ |
| Empty body: `for (...);` | ✅ |

## Example

```c
int main() {
    int i, s = 0;
    for (i = 0, s = 0; i < 3; i++, s += i);
    return s;  // 0+1+2+3 = 6
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `parse_for_stmt` | `src/parser.cpp:1258-1296` | Init/update via `parse_expression()` |
| Comma expression parsing | `src/parser.cpp` (`parse_expression`) | Walks `,` operator |

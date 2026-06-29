# Lesson 3005: Binary Literals (C23)

## Status: ✅ Complete | Standard: C23 | Effort: Easy

## Objective

Binary integer literal syntax (`0b` / `0B`).

## How It Works

The lexer recognises `0b` (or `0B`) as a binary-literal prefix and accumulates 0/1 digits. Digit separators (`'`) are accepted in the same loop:

```cpp
// src/lexer.cpp:280-288 — binary literal (0b prefix)
if (!is_at_end() && peek() == '0' && (peek_next() == 'b' || peek_next() == 'B')) {
    num += advance(); // '0'
    num += advance(); // 'b' or 'B'
    while (!is_at_end() && (peek() == '0' || peek() == '1' || peek() == '\'')) {
        if (peek() != '\'') num += advance(); else advance(); // skip separators
    }
    return Token(TokenType::INTEGER, num, start_line, start_column);
}
```

The token's text starts with `0b` and is later converted by the parser using `std::stoll(s.substr(2), nullptr, 2)`:

```cpp
// src/parser.cpp:1948
value = std::stoll(s.substr(2), nullptr, 2);
```

## Syntax

```c
int flags    = 0b1010'0101;   // 165
unsigned mask = 0b1111'0000;  // 240
char bits    = 0b1;           // 1
```

## Limitations

- `0b` with no following digits is silently accepted and yields `0` (no error).
- The bundled example in `3005-c23-binary-literals/src/example.c` does not actually use `0b`; it returns `10` from a plain `int x = 10;`.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|---|
| Lexer | `src/lexer.cpp:280-288` | Recognises `0b`/`0B` and accepts `'` |
| Parser | `src/parser.cpp:1948` | `std::stoll(s.substr(2), nullptr, 2)` |

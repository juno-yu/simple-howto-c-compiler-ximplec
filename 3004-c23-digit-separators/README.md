# Lesson 3004: Digit Separators (C23)

## Status: ✅ Complete | Standard: C23 | Effort: Easy

## Objective

Allow `'` between digits in numeric literals.

## How It Works

The lexer unconditionally drops `'` while scanning a number — for hex, binary, octal, decimal, and float forms. There is no validation that the quote is between two digits.

```cpp
// src/lexer.cpp:251-253 — hex literal
while (!is_at_end() && (std::isxdigit(peek()) || peek() == '\'')) {
    if (peek() != '\'') num += advance(); else advance();
}
```

```cpp
// src/lexer.cpp:284-286 — binary literal (0b prefix)
while (!is_at_end() && (peek() == '0' || peek() == '1' || peek() == '\'')) {
    if (peek() != '\'') num += advance(); else advance(); // skip separators
}
```

```cpp
// src/lexer.cpp:300-302 — decimal literal
while (!is_at_end() && (std::isdigit(peek()) || peek() == '\'')) {
    if (peek() != '\'') num += advance(); else advance();
}
```

The accumulated `num` string is later fed to `std::stoll` with the appropriate base (`src/parser.cpp:1946-1950`).

## Syntax

```c
int million = 1'000'000;
unsigned hex = 0xFF'FF'FF'FF;
unsigned bin = 0b1010'0101;
float pi = 3.141'592'653;
```

## Limitations

- Trailing/leading/consecutive quotes are accepted silently — no diagnostic for `1'`, `'1`, or `1''2`.
- The bundled example in `3004-c23-digit-separators/src/example.c` does not actually use digit separators; it returns `1000000 / 1000 = 1000` from a plain literal.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|---|
| Hex scan | `src/lexer.cpp:251-253` | Skips `'` |
| Binary scan | `src/lexer.cpp:284-286` | Skips `'` |
| Decimal scan | `src/lexer.cpp:300-302` | Skips `'` |
| Float scan | `src/lexer.cpp:307-309` | Skips `'` |
| Numeric conversion | `src/parser.cpp:1946-1950` | `std::stoll` per base |

# Lesson 3004: Digit Separators (C23)

## Status: 📋 Planned | Standard: C23 | Effort: Easy

## Objective

Allow single quotes in numeric literals for readability.

## Syntax

```c
int million = 1'000'000;
long long large = 123'456'789'ABC;
float pi = 3.141'592'653;
unsigned hex = 0xFF'FF'FF'FF;
unsigned bin = 0b1010'0101;
```

## Rules

- Single quote can appear between digits
- Cannot start or end with quote
- Cannot have two consecutive quotes
- Works in all numeric literals (decimal, hex, binary, float)

## Implementation Checklist

- [ ] Lexer: skip `'` in numeric literals
- [ ] Strip quotes before converting to value
- [ ] Support in integer literals (decimal, hex, binary)
- [ ] Support in float literals
- [ ] Test: `int x = 1'000;` → 1000
- [ ] Test: `int h = 0xFF'FF;` → 65535
- [ ] Test: error on `1'` (trailing quote)

## Flow Diagram

```mermaid
flowchart TD
    A[Source: 1'000'000] --> B[Lexer]
    B --> C{Numeric Literal?}
    C -->|Yes| D[Read Characters]
    D --> E{Is Quote?}
    E -->|Yes| F[Skip Quote]
    E -->|No| G[Accumulate Digit]
    F --> D
    G --> D
    D --> H[End of Literal]
    H --> I[Convert to Value]
    I --> J[Token::INT_LITERAL]
    J --> K[Parser]
    K --> L[AST: IntLiteral]
    L --> M[Codegen]
    M --> N[x86-64: mov $value, %rax]
```

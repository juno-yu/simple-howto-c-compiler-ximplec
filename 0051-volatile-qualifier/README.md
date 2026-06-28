# Lesson 0051: volatile Qualifier

## Status: 📋 Planned | Phase: System Integration | Effort: Easy (2-3h)

## Objective

Implement `volatile` to prevent optimization on memory accesses.

## volatile Qualifier Behavior

```mermaid
flowchart LR
    A[volatile Variable] --> B[Every read = memory load]
    A --> C[Every write = memory store]
    A --> D[No register caching]
    B --> E[No optimization allowed]
    C --> E
    D --> E
    E --> F[Hardware-mapped I/O]
    E --> G[Interrupt flags]
    E --> H[Memory-mapped registers]
```

## volatile vs const

```mermaid
flowchart TD
    A[Type Qualifiers] --> B[const]
    A --> C[volatile]
    B --> D[Read-only - compiler enforces]
    C --> E[Always re-read from memory]
    D --> F[Cannot modify]
    E --> G[Compiler must not cache]
    A --> H[const volatile]
    H --> I[Read-only but always from memory]
    H --> J[Hardware status registers]
```

## Implementation Checklist

- [ ] Parse `volatile` keyword
- [ ] Disable register caching for volatile reads
- [ ] Generate memory load/store for every volatile access
- [ ] Test: volatile read generates memory load every iteration

## Implementation Details

The `volatile` qualifier is implemented across the lexer, token definitions, and parser. The compiler recognizes `volatile` as a type qualifier and parses it alongside `const`, `static`, and `extern`.

| Component | File | Line | Description |
|-----------|------|------|-------------|
| Token type | `src/token.h` | 49 | `KW_VOLATILE` enum value definition |
| Token name | `src/lexer.cpp` | 38 | Maps `KW_VOLATILE` to string `"volatile"` |
| Keyword map | `src/lexer.cpp` | 122 | Registers `"volatile"` as `KW_VOLATILE` keyword |
| Type specifier | `src/parser.cpp` | 73 | Recognizes `KW_VOLATILE` in `is_type_specifier()` |
| Qualifier parse | `src/parser.cpp` | 94-95 | Parses `volatile` keyword and appends to type string |
| Type string | `src/parser.cpp` | 87-105 | `parse_type_specifier()` handles qualifiers in any order |
| Test coverage | `tests/test_volatile_qualifier.cpp` | 1-87 | Tests volatile int, volatile pointer, const volatile |

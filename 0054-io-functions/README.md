# Lesson 0054: I/O Functions

## Status: 📋 Planned | Phase: Stdlib Tier A | Effort: Medium (8-12h)

## Objective

Implement printf, puts, putchar, and basic I/O.

## I/O Functions Overview

```mermaid
flowchart TD
    A[I/O Functions] --> B[putchar]
    A --> C[puts]
    A --> D[putint]
    A --> E[printf]

    B -->|"putchar(c)"| F[Write single char to stdout]
    C -->|"puts(s)"| G[Write string + newline]
    D -->|"putint(n)"| H[Convert int to string and write]
    E -->|"printf(fmt, ...)"| I[Formatted output]

    I --> J["%d - integer"]
    I --> K["%s - string"]
    I --> L["%c - character"]
    I --> M["%x - hexadecimal"]
    I --> N["%% - literal %"]
```

## printf Format String Parsing

```mermaid
flowchart LR
    A["printf(\"hi %d %s\\n\", 42, \"wow\")"] --> B[Parse format string]
    B --> C[Output: hi ]
    C --> D["%d → 42"]
    D --> E[Output:  ]
    E --> F["%s → wow"]
    F --> G[Output: \n]
    G --> H["Final: hi 42 wow\n"]
```

## Functions

| Function | Complexity |
|----------|------------|
| `putchar(c)` | Trivial |
| `puts(s)` | Easy |
| `putint(n)` | Easy |
| `printf(fmt, ...)` | Hard |

## Implementation Checklist

- [ ] Implement putchar via write syscall
- [ ] Implement puts (string + newline)
- [ ] Implement putint (int to string conversion)
- [ ] Implement printf with `%d`, `%s`, `%c`, `%x`
- [ ] Handle format string parsing
- [ ] Handle varargs
- [ ] Test: `printf("hello %s %d\n", "world", 42);`

## Implementation Details

I/O functions like `printf`, `puts`, and `putchar` are declared via `extern` with variadic (`...`) parameter support. The compiler's variadic parameter parsing (`ELLIPSIS` token) allows flexible argument counts. Function calls pass arguments via System V ABI registers.

| Component | File | Line | Description |
|-----------|------|------|-------------|
| ELLIPSIS token | `src/token.h` | 52 | `ELLIPSIS` token type for variadic params |
| Variadic parse | `src/parser.cpp` | 596-599 | Parses `...` as variadic parameter |
| Extern parse | `src/parser.cpp` | 218-248 | Parses `extern` function declarations |
| Varargs call | `src/codegen.cpp` | 838-854 | Passes up to 6 args via registers |
| String literal | `src/codegen.cpp` | 929-936 | Emits format strings to `.rodata` |
| Func prologue | `src/codegen.cpp` | 73-88 | Standard function prologue/epilogue |
| Test coverage | `tests/test_io_functions.cpp` | 1-102 | Tests printf, scanf, putchar, puts declarations |

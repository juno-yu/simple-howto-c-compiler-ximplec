# Lesson 3013: [[nodiscard]] (C23)

## Status: ✅ Complete | Standard: C23 | Effort: Easy

## Objective

Warn when function return value is ignored.

## Syntax

```c
[[nodiscard]] int critical_operation(void);
[[nodiscard("error code")]] int error_prone(void);

// Warning: ignoring return value
critical_operation();  // compiler warns
```

## Use Cases

- Error codes: `[[nodiscard]] int fclose(FILE *);`
- Resource allocation: `[[nodiscard]] void *malloc(size_t);`
- Pure functions: `[[nodiscard]] int compute(int);`

## Implementation Checklist

- [ ] Parse `[[nodiscard]]` on functions
- [ ] Parse `[[nodiscard("msg")]]` with message
- [ ] Warn when return value is discarded
- [ ] Don't warn in void context: `(void)f();`
- [ ] Test: `[[nodiscard]] int f(void); f();` → warning

## Flow Diagram

```mermaid
flowchart TD
    A[Source: [[nodiscard]] int f] --> B[Lexer]
    B --> C{Double bracket?}
    C -->|Yes| D[Parse attribute]
    C -->|No| E[Normal token]
    D --> F{Attribute name: nodiscard?}
    F -->|Yes| G[Set NODISCARD flag]
    F -->|No| H[Ignore unknown attribute]
    G --> I[Attach to function declaration]
    H --> I
    I --> J[Parser]
    J --> K[AST: FuncDecl with nodiscard]
    K --> L{Function call without using result?}
    L -->|Yes| M[Generate warning]
    L -->|No| N[Normal codegen]
    M --> O[Assembly with warning metadata]
    N --> O
```

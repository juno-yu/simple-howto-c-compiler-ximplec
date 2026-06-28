# Lesson 0021: Extern Declarations

## Status: 📋 Planned | Phase: String & Memory | Effort: Easy (2-3h)

## Objective

Allow calling external library functions via declarations.

## Extern Declaration Flow

```mermaid
flowchart TD
    A["extern int printf(...)"] --> B[Parse extern keyword]
    B --> C[Store in symbol table as external]
    C --> D[Do NOT generate code]
    D --> E[Allow calls to extern functions]
    E --> F["Call site generates: call printf"]
    F --> G[Linker resolves symbol]
```

## Implementation Checklist

- [ ] Parse `extern int printf(const char *, ...);`
- [ ] Store extern declarations in symbol table
- [ ] Do NOT generate code for extern declarations
- [ ] Allow calls to extern functions
- [ ] Test: declare extern, call, link with gcc

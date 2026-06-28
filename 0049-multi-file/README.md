# Lesson 0049: Multi-File Compilation

## Status: 📋 Planned | Phase: System Integration | Effort: Medium (8-12h)

## Objective

Implement separate compilation and linking.

## Multi-File Compilation Flow

```mermaid
flowchart TD
    A["file1.c, file2.c"] --> B[Compile each to object file]
    B --> C["file1.o, file2.o"]
    C --> D[Symbol table extraction]
    D --> E[Linker resolves references]
    E --> F{All symbols resolved?}
    F -->|Yes| G[Produce executable]
    F -->|No| H[Linker error]

    I["Symbol resolution"] --> J["extern declarations"]
    I --> K["Global symbols visible across files"]
    I --> L["Static symbols file-local"]
```

## Implementation Checklist

- [ ] Compile each `.c` file to `.o` object file
- [ ] Support `-c` flag for compile-only
- [ ] Link multiple `.o` files
- [ ] Symbol resolution across files
- [ ] Test: split program into 2 files, compile and link

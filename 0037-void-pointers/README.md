# Lesson 0037: void Pointers

## Status: ✅ Complete | Phase: Advanced Types | Effort: Easy (2-3h)

## Objective

Implement `void*` as generic pointer type.

## Implementation Checklist

- [ ] Treat `void*` as pointer type (size = 8)
- [ ] Allow implicit conversion to/from `void*`
- [ ] Allow casting between pointer types
- [ ] Test: `void *p = &x; int *q = (int*)p; return *q;`

## Architecture

```mermaid
flowchart TD
    A["void* Declaration"] --> B["Allocate pointer storage"]
    B --> C["Size = 8 bytes on x86-64"]

    D["Implicit Conversion"] --> E{"Source type?"}
    E -->|Typed pointer| F["Convert to void*"]
    E -->|void*| G["Convert to typed pointer"]

    H["Explicit Cast"] --> I["Cast expression"]
    I --> J["Verify pointer type"]
    J --> K["Generate load/store via cast type"]

    L["void* Arithmetic Check"] --> M{"Arithmetic on void*?"}
    M -->|Yes| N["Error: pointer arithmetic on void*"]
    M -->|No| O["Allow dereference after cast"]

    P["Common Patterns"] --> Q["malloc returns void*"]
    P --> R["qsort callback context"]
    P --> S["Generic container"]

    style F fill:#ffd,stroke:#333
    style G fill:#ffd,stroke:#333
    style N fill:#fdd,stroke:#333
```

## Implementation Details

| Component | File | Lines | Description |
|-----------|------|-------|-------------|
| Type specifier parsing (void) | `src/parser.cpp` | 87-183 | `parse_type_specifier()` handles `void` keyword and pointer stars |
| Pointer type detection | `src/parser.cpp` | 177-180 | Appends `*` to type string for pointer qualifiers |
| Deref expression AST | `src/ast.h` | 480-485 | `DerefExprNode` for `*p` dereference operations |
| Type size calculation | `src/codegen.cpp` | 1197-1214 | Returns 8 bytes for `void*` and any pointer type |
| Deref codegen | `src/codegen.cpp` | 907-910 | `mov (%rax), %rax` to dereference pointer |
| Cast expression support | `src/codegen.cpp` | 832-836 | `(int*)vp` cast expressions pass through to operand |
| Identifier address loading | `src/codegen.cpp` | 942-966 | `lea offset(%rbp), %rax` for pointer variables |

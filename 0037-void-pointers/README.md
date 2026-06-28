# Lesson 0037: void Pointers

## Status: 📋 Planned | Phase: Advanced Types | Effort: Easy (2-3h)

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

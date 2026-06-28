# Lesson 0050: Static Linkage

## Status: ✅ Complete | Phase: System Integration | Effort: Easy (2-3h)

## Objective

Implement `static` for file-local symbols.

## Static Linkage Processing

```mermaid
flowchart TD
    A["static int x = 0;"] --> B[Parse static qualifier]
    B --> C{Declaration context?}
    C -->|File scope| D[Omit .globl directive]
    C -->|Function scope| E[Static local: persistent storage]
    D --> F[Symbol not exported]
    E --> G[Retains value between calls]

    H["Visibility rules"] --> I["static: file-local only"]
    H --> J["extern: explicitly external"]
    H --> K["default: global visibility"]
```

## Implementation Checklist

- [ ] Parse `static` on functions and variables
- [ ] Omit `.globl` for static symbols
- [ ] Static local variables (persistent)
- [ ] Test: `static void helper() {}` not visible outside file

# Lesson 0026: Pointer Arithmetic

## Status: 📋 Planned | Phase: Data Structures | Effort: Medium (4-6h)

## Objective

Implement `p + n`, `p - n`, `p++`, `p--`.

## Implementation Checklist

- [ ] Pointer + integer: `p + n` → `p + n * sizeof(*p)`
- [ ] Pointer - integer: `p - n` → `p - n * sizeof(*p)`
- [ ] Pointer difference: `p - q` → `(p - q) / sizeof(*p)`
- [ ] Pointer comparison
- [ ] Test: `int a[3] = {10, 20, 30}; int *p = a; return *(p + 1);` → 20

## Architecture

```mermaid
flowchart TD
    A["Pointer Arithmetic"] --> B["p + n"]
    A --> C["p - n"]
    A --> D["p++ / p--"]
    A --> E["p - q"]

    B --> F["Multiply n by sizeof(*p)"]
    F --> G["Add byte offset to address"]

    C --> H["Multiply n by sizeof(*p)"]
    H --> I["Subtract byte offset from address"]

    D --> J["Auto-increment by sizeof(*p)"]

    E --> K["Compute element difference"]
    K --> L["Divide byte difference by sizeof(*p)"]

    G --> M["Dereference result"]
    I --> M
    L --> M
```

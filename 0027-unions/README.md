# Lesson 0027: Unions

## Status: 📋 Planned | Phase: Data Structures | Effort: Medium (4-6h)

## Objective

Implement union types with overlapping member storage.

## Implementation Checklist

- [ ] Parse `union` keyword
- [ ] Calculate union size = max(member sizes)
- [ ] All members share base address
- [ ] Union member access (same as struct)
- [ ] Test: `union { int i; float f; } u; u.i = 42; return u.i;` → 42

## Architecture

```mermaid
flowchart TD
    A["Union Declaration"] --> B["Calculate size = max of all members"]
    B --> C["All members share base address"]

    D["Union Access (u.field)"] --> E["Compute offset of field"]
    E --> F["Load/store at base + offset"]

    G["Memory Layout"] --> H["Member 1: offset 0, size N1"]
    G --> I["Member 2: offset 0, size N2"]
    G --> J["Member 3: offset 0, size N3"]
    H --> K["Total size = max(N1, N2, N3)"]
    I --> K
    J --> K

    style G fill:#f9f,stroke:#333
    style K fill:#bbf,stroke:#333
```

# Lesson 0038: Designated Initializers (C99)

## Status: 📋 Planned | Phase: Advanced Types | Effort: Medium (6-8h)

## Objective

Implement `.field = value` and `[index] = value` in initializer lists.

## Implementation Checklist

- [ ] Parse `.field = value` in struct initializers
- [ ] Parse `[index] = value` in array initializers
- [ ] Fill unspecified members with zeros
- [ ] Handle nested designators
- [ ] Error on duplicate designators
- [ ] Test: `struct { int a; int b; } s = { .b = 42 };`

## Architecture

```mermaid
flowchart TD
    A["Designated Initializer Parse"] --> B{"Designator type?"}
    B -->|".field"| C["Struct field designator"]
    B -->|"[idx]"| D["Array index designator"]

    C --> E["Look up field in struct type"]
    E --> F["Set initializer at field offset"]

    D --> G["Compute array index offset"]
    G --> H["Set initializer at index"]

    I["Fill Zeros"] --> J["For each uninitialized member"]
    J --> K["Emit zero initializer"]

    L["Mixed Positional + Designated"] --> M["Track current position"]
    M --> N["Positional fills from current"]
    N --> O["Designated jumps to target"]

    P["Error: Duplicate Designator"] --> Q["Report compile error"]

    style C fill:#ffd,stroke:#333
    style D fill:#ffd,stroke:#333
    style Q fill:#fdd,stroke:#333
```

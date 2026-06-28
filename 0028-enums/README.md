# Lesson 0028: Enums

## Status: 📋 Planned | Phase: Data Structures | Effort: Easy (4-6h)

## Objective

Implement `enum` for named integer constants.

## Implementation Checklist

- [ ] Parse `enum Name { A, B, C }`
- [ ] Auto-assign values (0, 1, 2, ...)
- [ ] Support explicit values: `A = 10`
- [ ] Treat enums as integers
- [ ] Test: `enum Color { RED, GREEN, BLUE }; return GREEN;` → 1

## Architecture

```mermaid
flowchart TD
    A["Enum Declaration"] --> B["Parse enum name and values"]
    B --> C["Assign integer values"]

    C --> D{"Explicit value?"}
    D -->|Yes| E["Use specified value"]
    D -->|No| F["Increment from previous"]

    E --> G["Register in symbol table"]
    F --> G

    H["Enum Usage"] --> I["Look up enum value"]
    I --> J["Replace with integer literal"]

    K["enum Color { RED, GREEN, BLUE }"] --> L["RED = 0"]
    K --> M["GREEN = 1"]
    K --> N["BLUE = 2"]

    O["enum Status { OK=200, FAIL=500 }"] --> P["OK = 200"]
    O --> Q["FAIL = 500"]

    style D fill:#ffd,stroke:#333
```

# Lesson 0040: Bitfields

## Status: 📋 Planned | Phase: Advanced Types | Effort: Medium (4-6h)

## Objective

Implement bitfield members in structs.

## Implementation Checklist

- [ ] Parse `unsigned int field : width;`
- [ ] Calculate bitfield storage units
- [ ] Generate bit manipulation code for access
- [ ] Handle bitfield packing across storage units
- [ ] Test: `struct { unsigned a:1; unsigned b:3; } s; s.b = 5;`

## Architecture

```mermaid
flowchart TD
    A["Bitfield Declaration"] --> B["Parse field name and width"]
    B --> C["Calculate bit offset within storage unit"]
    C --> D["Pack into storage unit"]

    D --> E{"Fits in current unit?"}
    E -->|Yes| F["Add to current storage unit"]
    E -->|No| G["Start new storage unit"]
    G --> F

    H["Bitfield Read Access"] --> I["Load storage unit"]
    I --> J["Shift right by bit offset"]
    J --> K["Mask by field width"]

    L["Bitfield Write Access"] --> M["Load storage unit"]
    M --> N["Clear field bits with mask"]
    N --> O["Shift new value left"]
    O --> P["OR into storage unit"]

    Q["Storage Layout"] --> R["unsigned a:1  → bit 0"]
    Q --> S["unsigned b:3  → bits 1-3"]
    Q --> T["unsigned c:4  → bits 4-7"]
    Q --> U["All fit in 1 byte"]

    style E fill:#ffd,stroke:#333
    style K fill:#ffd,stroke:#333
    style P fill:#ffd,stroke:#333
```

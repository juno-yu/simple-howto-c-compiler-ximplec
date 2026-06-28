# Lesson 0040: Bitfields

## Status: ✅ Complete | Phase: Advanced Types | Effort: Medium (4-6h)

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

## Implementation Details

| Component | File | Lines | Description |
|-----------|------|-------|-------------|
| Bitfield syntax parsing | `src/parser.cpp` | 285-289 | `match(COLON)` after field name, skips integer width token |
| Struct field parsing | `src/parser.cpp` | 276-295 | `parse_type_specifier()` + identifier + optional `: width` |
| StructDeclNode fields | `src/ast.h` | 233-240 | `vector<ASTPtr> fields` stores StructFieldNode children |
| StructFieldNode structure | `src/ast.h` | 224-231 | Stores `type_name` and `name` for each field |
| Struct layout computation | `src/codegen.cpp` | 383-399 | Iterates fields, computes offset incrementally by `get_type_size()` |
| FieldInfo tracking | `src/codegen.cpp` | 1196-1230 | `struct_layouts_` map stores offset and size per field |
| Member access codegen | `src/codegen.cpp` | 338-377 | `compute_member_address()` adds field offset to base address |

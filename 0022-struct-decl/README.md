# Lesson 0022: Struct Declarations

## Status: 📋 Planned | Phase: Data Structures | Effort: Hard (8-12h)

## Objective

Parse and store struct type definitions.

## Struct Declaration and Layout

```mermaid
classDiagram
    class StructType {
        +string name
        +vector~Member~ members
        +int total_size
    }
    class Member {
        +string name
        +Type* type
        +int offset
    }
    class Type {
        +int size
    }
    StructType *-- Member
    Member --> Type

    note for StructType "struct Point {\n  int x; // offset 0\n  int y; // offset 4\n};\ntotal_size = 8"
```

## Implementation Checklist

- [ ] Parse `struct Name { type member; ... }`
- [ ] Calculate member offsets with alignment
- [ ] Calculate total struct size with padding
- [ ] Register struct types in type system
- [ ] Support nested structs
- [ ] Support forward struct declarations
- [ ] Test: `struct Point { int x; int y; }; sizeof(struct Point)` → 8

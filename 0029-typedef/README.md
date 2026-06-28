# Lesson 0029: typedef

## Status: 📋 Planned | Phase: Data Structures | Effort: Medium (6-8h)

## Objective

Implement type aliases.

## Implementation Checklist

- [ ] Parse `typedef int MyInt;`
- [ ] Parse `typedef struct {...} Name;`
- [ ] Register typedef names in symbol table
- [ ] Recognize typedef'd names as type specifiers
- [ ] Test: `typedef int MyInt; MyInt x = 42; return x;` → 42

## Architecture

```mermaid
flowchart TD
    A["typedef int MyInt;"] --> B["Parse source type"]
    B --> C["Parse alias name"]
    C --> D["Register alias -> source in type table"]

    E["Usage: MyInt x;"] --> F["Look up 'MyInt' in type table"]
    F --> G["Resolve to 'int'"]
    G --> H["Treat as regular int declaration"]

    I["typedef struct {...} Name;"] --> J["Parse struct body"]
    J --> K["Create struct type"]
    K --> L["Register Name -> struct type"]

    M["typedef aliases"] --> N["int -> MyInt"]
    M --> O["struct S -> SType"]
    M --> P["int* -> IntPtr"]

    style D fill:#bfb,stroke:#333
    style G fill:#bbf,stroke:#333
```

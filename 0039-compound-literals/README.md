# Lesson 0039: Compound Literals (C99)

## Status: 📋 Planned | Phase: Advanced Types | Effort: Medium (4-6h)

## Objective

Implement `(type){init-list}` for unnamed objects.

## Implementation Checklist

- [ ] Parse `(type){init-list}` expression syntax
- [ ] Create anonymous object in current scope
- [ ] Generate stack allocation for block-scope literals
- [ ] Support struct, array, and scalar compound literals
- [ ] Test: `return (struct Point){1, 2}.x;` → 1

## Architecture

```mermaid
flowchart TD
    A["Compound Literal Expression"] --> B["Parse type specifier"]
    B --> C["Parse initializer list"]
    C --> D["Create anonymous object"]

    D --> E{"Scope?"}
    E -->|Block scope| F["Allocate on stack"]
    E -->|File scope| G["Allocate in static data"]

    F --> H["Initialize fields from list"]
    G --> H

    H --> I["Return pointer/value to object"]

    J["Compound Literal as Value"] --> K["Address-of yields pointer"]
    J --> L["Value copy for assignment"]

    M["Compound Literal as Argument"] --> N["Pass by value to function"]
    M --> O["Pass pointer if parameter is pointer"]

    P["int[]{1,2,3}"] --> Q["Array compound literal"]
    R["struct{1,2}"] --> S["Struct compound literal"]

    style F fill:#ffd,stroke:#333
    style I fill:#ffd,stroke:#333
```

# Lesson 0014: sizeof Operator

## Status: 📋 Planned | Phase: Type System | Effort: Easy (4-6h)

## Objective

Implement `sizeof(type)` and `sizeof(expr)`.

## sizeof Operator Forms

```mermaid
flowchart TD
    A[sizeof] --> B[Type Form: sizeof type]
    A --> C[Expression Form: sizeof expr]
    B --> D["sizeof(int) → 4"]
    B --> E["sizeof(char) → 1"]
    B --> F["sizeof(void*) → 8"]
    C --> G["sizeof(x) → size of x's type"]
    C --> H["sizeof(*p) → size of pointed type"]
```

## Implementation Checklist

- [ ] Parse `sizeof(type)` - type form
- [ ] Parse `sizeof(expr)` - expression form
- [ ] Return `IntegerLiteralNode` with computed size
- [ ] Handle: `sizeof(char)` → 1, `sizeof(int)` → 4, `sizeof(void*)` → 8
- [ ] Support pointer types: `sizeof(int*)` → 8
- [ ] Support struct types: `sizeof(struct Point)` → computed
- [ ] Test: `return sizeof(int);` → 4
- [ ] Test: `return sizeof(void*);` → 8

# Lesson 0045: _Generic Selection (C11)

## Status: ✅ Complete | Phase: Float & Advanced | Effort: Hard (8-12h)

## Objective

Implement compile-time type-based dispatch.

## Generic Selection Processing

```mermaid
flowchart TD
    A["_Generic(expr, type1: expr1, ...)"] --> B[Evaluate controlling expr type]
    B --> C{Find matching type}
    C -->|Match found| D["Return associated expression"]
    C -->|No match, has default| E["Return default expression"]
    C -->|No match, no default| F[Compile error]

    G["Type matching"] --> H["Exact type match required"]
    G --> I["No implicit conversions"]
    G --> J["Associations evaluated at compile time"]
    G --> K["Result is typed expression"]
```

## Implementation Checklist

- [ ] Parse `_Generic(expr, type: expr, ..., default: expr)`
- [ ] Evaluate type of controlling expression
- [ ] Find matching type in association list
- [ ] Return the matched expression (compile-time)
- [ ] Test: type dispatch macro

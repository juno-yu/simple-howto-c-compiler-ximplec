# Lesson 0044: _Static_assert (C11)

## Status: 📋 Planned | Phase: Float & Advanced | Effort: Easy (2-3h)

## Objective

Implement compile-time assertions.

## Static Assert Processing

```mermaid
flowchart TD
    A["_Static_assert(expr, msg)"] --> B{Parse statement}
    B --> C[Evaluate constexpr expr]
    C --> D{Result is zero?}
    D -->|Yes| E["Compile error: msg"]
    D -->|No| F[Continue compilation]

    G["Constant expression types"] --> H["Integer literals"]
    G --> I["sizeof expressions"]
    G --> J["Enum constants"]
    G --> K["Preprocessor macros"]
```

## Implementation Checklist

- [ ] Parse `_Static_assert(constexpr, "message")`
- [ ] Evaluate constant expression at compile time
- [ ] Report error with message if assertion fails
- [ ] Test: `_Static_assert(1, "always passes");`
- [ ] Test: `_Static_assert(0, "fails");` → compile error

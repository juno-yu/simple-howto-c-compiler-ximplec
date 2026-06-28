# Lesson 1012: static_assert (C11 Optional)

## Status: ✅ Complete | Standard: C11 | Effort: Easy

## Objective

Alias for `_Static_assert` via `<assert.h>`.

## Static Assert Alias Flow

```mermaid
graph TD
    A["static_assert(expr, msg)"] --> B{C11 implementation}
    B -->|"<assert.h>"| C["Macro expands to _Static_assert"]
    B -->|C23| D["static_assert is a keyword"]
    C --> E["_Static_assert(expr, msg)"]
    E --> F{expr is true?}
    F -->|Yes| G["Compile continues"]
    F -->|No| H["Compile error with msg"]
```

## Notes

- `<assert.h>` may define `static_assert` as macro
- C11 requires `_Static_assert` as keyword
- C23 makes `static_assert` a keyword

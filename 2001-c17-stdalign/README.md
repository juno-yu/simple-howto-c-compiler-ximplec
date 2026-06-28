# Lesson 2001: stdalign.h (C17)

## Status: ✅ Complete | Standard: C17 | Effort: Trivial

## Objective

Provide `alignas` and `alignof` macros.

## C17 Notes

- No changes from C11
- `<stdalign.h>` provides: `alignas`, `alignof`
- Maps to `_Alignas`, `_Alignof`

## Implementation

- Header defines: `#define alignas _Alignas`
- Header defines: `#define alignof _Alignof`

## Processing Flow

```mermaid
flowchart TD
    A["alignas(N) decl"] --> B[Preprocessor expands to _Alignas]
    B --> C[Parse alignment specifier]
    C --> D[Validate N is power of 2]
    D --> E[Apply alignment to declaration]
    E --> F[Layout calculation with alignment]

    G["alignof(type)"] --> H[Preprocessor expands to _Alignof]
    H --> I[Look up type alignment requirement]
    I --> J[Return alignment value as size_t]
```

# Lesson 2004: typeof (C17)

## Status: 📋 Planned | Standard: C17 | Effort: Easy

## Objective

GCC extension `typeof` (standardized in C23).

## Notes

- C17 doesn't standardize `typeof`
- This lesson covers GCC extension support
- C23 makes it official

## Implementation

- Parse `typeof(expr)` as type specifier
- Use in variable declarations

## Processing Flow

```mermaid
flowchart TD
    A["typeof(expr)"] --> B[Parse typeof keyword]
    B --> C[Evaluate expression type]
    C --> D[Resolve to concrete type T]
    D --> E["typeof(expr) resolves to T"]
    E --> F[Use T in declaration context]

    G["typeof(x) y = val;"] --> H[Parse typeof in declaration]
    H --> I[Determine type of x]
    I --> J["Declare y with same type"]
    J --> K[Initialize y with val]
```

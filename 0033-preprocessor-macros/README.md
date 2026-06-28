# Lesson 0033: Preprocessor Macros

## Status: 📋 Planned | Phase: Preprocessor | Effort: Medium (8-12h)

## Objective

Implement `#define` for object-like and function-like macros.

## Macro Expansion Flow

```mermaid
flowchart TD
    A["Source code"] --> B[Preprocessor phase]
    B --> C{"#define found?"}
    C -->|Yes| D[Parse macro name]
    C -->|No| E[Pass through unchanged]

    D --> F{"Has parentheses?"}
    F -->|No| G["Object-like: #define NAME value"]
    F -->|Yes| H["Function-like: #define NAME(args) replacement"]

    G --> I[Store NAME → value mapping]
    H --> J[Store NAME → args, replacement]

    K["Token scanning"] --> L{"Token matches macro?"}
    L -->|Yes| M[Perform text substitution]
    L -->|No| N[Keep token as-is]

    M --> O{"Function-like macro?"}
    O -->|Yes| P[Match arguments in parens]
    P --> Q[Replace #param with arg text]
    O -->|No| R[Replace directly]

    Q --> S[Recursive expansion]
    R --> S
    S --> T{"More macros to expand?"}
    T -->|Yes| K
    T -->|No| U[Output expanded tokens]
```

## Implementation Checklist

- [ ] Add preprocessor phase to compilation pipeline
- [ ] Parse `#define NAME value` (object-like)
- [ ] Parse `#define NAME(args) replacement` (function-like)
- [ ] Token-level replacement
- [ ] Handle `#undef NAME`
- [ ] Recursive expansion
- [ ] Test: `#define PI 3.14` then `return PI;`

# Lesson 3014: Predefined Macros (C23)

## Status: 📋 Planned | Standard: C23 | Effort: Easy

## Objective

Additional predefined macros.

## New Macros in C23

| Macro | Description |
|-------|-------------|
| `__STDC_VERSION__` | `202311L` for C23 |
| `__VA_OPT__(,)` | Optional comma in variadic macros |
| `__cplusplus` | C++ compatibility (if applicable) |

## Standard Macros (all versions)

| Macro | Description |
|-------|-------------|
| `__FILE__` | Current filename |
| `__LINE__` | Current line number |
| `__DATE__` | Compilation date |
| `__TIME__` | Compilation time |
| `__STDC__` | Conforming implementation |
| `__STDC_HOSTED__` | Hosted implementation |

## Implementation Checklist

- [ ] Define `__STDC_VERSION__` as `202311L`
- [ ] Implement `__VA_OPT__(,)` for variadic macros
- [ ] Ensure all standard macros are defined
- [ ] Test: `#if __STDC_VERSION__ >= 202311L`

## Flow Diagram

```mermaid
flowchart TD
    A[Source: #if __STDC_VERSION__ >= 202311L] --> B[Lexer]
    B --> C{Preprocessor directive?}
    C -->|Yes| D[Parse macro]
    C -->|No| E[Normal token]
    D --> F{Is predefined macro?}
    F -->|Yes| G[Look up macro value]
    F -->|No| H[Look up user macro]
    G --> I{__STDC_VERSION__?}
    I -->|Yes| J[Return 202311L]
    G --> K{__VA_OPT__?}
    K -->|Yes| L[Handle optional comma]
    H --> M[Expand macro]
    J --> N[Preprocessor]
    L --> N
    M --> N
    N --> O[AST: expanded code]
    O --> P[Codegen]
    P --> Q[Assembly]
```

# Lesson 3007: Attributes (C23)

## Status: 📋 Planned | Standard: C23 | Effort: Medium

## Objective

Standardized attribute syntax `[[attr]]`.

## Syntax

```c
[[noreturn]] void abort(void);
[[nodiscard]] int important_func(void);
[[maybe_unused]] int x = 42;
[[deprecated]] void old_func(void);
[[fallthrough]] case 1:
```

## Standard Attributes

| Attribute | Purpose |
|-----------|---------|
| `[[noreturn]]` | Function never returns |
| `[[nodiscard]]` | Warn if return ignored |
| `[[maybe_unused]]` | Suppress unused warning |
| `[[deprecated]]` | Mark as deprecated |
| `[[fallthrough]]` | Suppress fallthrough warning |

## Implementation Checklist

- [ ] Parse `[[attribute]]` syntax
- [ ] Parse `[[namespace::attr]]` syntax
- [ ] Implement standard attributes
- [ ] Ignore unknown attributes
- [ ] Test: `[[nodiscard]] int f(void);` warns on ignored return

## Flow Diagram

```mermaid
flowchart TD
    A[Source: [[nodiscard]] int f] --> B[Lexer]
    B --> C{Double Bracket?}
    C -->|Yes| D[Parse Attribute]
    C -->|No| E[Normal Token]
    D --> F[Read Attribute Name]
    F --> G{Known Attribute?}
    G -->|nodiscard| H[Set NODISCARD Flag]
    G -->|maybe_unused| I[Set MAYBE_UNUSED Flag]
    G -->|deprecated| J[Set DEPRECATED Flag]
    G -->|fallthrough| K[Set FALLTHROUGH Flag]
    G -->|noreturn| L[Set NORETURN Flag]
    G -->|Unknown| M[Ignore Attribute]
    H --> N[Attach to Declaration]
    I --> N
    J --> N
    K --> N
    L --> N
    M --> N
    N --> O[Parser]
    O --> P[AST: Decl with Attributes]
    P --> Q[Codegen]
    Q --> R[Assembly with Metadata]
```

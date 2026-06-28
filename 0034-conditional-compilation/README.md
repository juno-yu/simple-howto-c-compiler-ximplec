# Lesson 0034: Conditional Compilation

## Status: ✅ Complete | Phase: Preprocessor | Effort: Medium (6-8h)

## Objective

Implement `#ifdef`, `#ifndef`, `#if`, `#elif`, `#else`, `#endif`.

## Conditional Compilation Flow

```mermaid
flowchart TD
    A["Preprocessor directive"] --> B{"Directive type?"}

    B -->|"#ifdef NAME"| C{"NAME defined?"}
    C -->|Yes| D[Include block]
    C -->|No| E[Skip block]

    B -->|"#ifndef NAME"| F{"NAME defined?"}
    F -->|No| G[Include block]
    F -->|Yes| H[Skip block]

    B -->|"#if expr"| I{Evaluate expr}
    I -->|True| J[Include block]
    I -->|False| K[Skip block]

    B -->|"#elif expr"| L{Previous skipped?}
    L -->|Yes| M{Evaluate expr}
    M -->|True| N[Include block]
    M -->|False| O[Skip block]
    L -->|No| P[Skip block]

    B -->|"#else"| Q{Any previous included?}
    Q -->|No| R[Include block]
    Q -->|Yes| S[Skip block]

    B -->|"#endif"| T[End of conditional]

    D --> U[Continue processing]
    G --> U
    J --> U
    N --> U
    R --> U
```

## Implementation Checklist

- [ ] Parse `#ifdef NAME` / `#ifndef NAME`
- [ ] Parse `#if constexpr_expr`
- [ ] Parse `#elif`, `#else`, `#endif`
- [ ] Support `defined(NAME)` operator
- [ ] Nested conditionals
- [ ] Handle `#pragma once`
- [ ] Test: `#ifdef DEBUG ... #else ... #endif`

## Implementation Details

**Status: Not yet implemented.** No preprocessor infrastructure exists in `src/`. Conditional compilation requires the preprocessor phase first.

| Feature | File | Description |
|---------|------|-------------|
| Conditional evaluator | `src/preprocessor.cpp` *(new)* | `#ifdef`/`#ifndef`/`#if`/`#elif`/`#else`/`#endif` handling |
| Macro table | `src/preprocessor.h` *(new)* | Tracks `defined()` state |
| Lexer tokens | `src/token.h` | `KW_IFDEF`, `KW_IFNDEF`, `KW_IF`, `KW_ELIF`, `KW_ELSE`, `KW_ENDIF` needed |

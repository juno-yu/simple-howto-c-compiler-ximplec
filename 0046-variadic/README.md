# Lesson 0046: Variadic Functions

## Status: ✅ Complete | Phase: Float & Advanced | Effort: Hard (12-16h)

## Objective

Implement functions with variable number of arguments.

## Variadic Function Processing

```mermaid
flowchart TD
    A["int sum(int count, ...)"] --> B[Parse variadic parameter list]
    B --> C[Save register args to stack]
    C --> D[va_start: locate first vararg]
    D --> E[va_arg: read next argument]
    E --> F{More arguments?}
    F -->|Yes| E
    F -->|No| G[va_end: cleanup]
    G --> H[Return result]

    I["va_list internals"] --> J["Stack pointer tracking"]
    I --> K["Register save area"]
    I --> L["Offset-based access"]
```

## Implementation Checklist

- [ ] Parse `...` in parameter list
- [ ] Parse `va_list`, `va_start`, `va_arg`, `va_end`
- [ ] Save register arguments to stack
- [ ] Implement `va_start`, `va_arg`, `va_end`
- [ ] Test: implement `sum(int count, ...)` function
- [ ] Test: implement basic `printf`-like function

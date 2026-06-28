# Lesson 0051: volatile Qualifier

## Status: 📋 Planned | Phase: System Integration | Effort: Easy (2-3h)

## Objective

Implement `volatile` to prevent optimization on memory accesses.

## volatile Qualifier Behavior

```mermaid
flowchart LR
    A[volatile Variable] --> B[Every read = memory load]
    A --> C[Every write = memory store]
    A --> D[No register caching]
    B --> E[No optimization allowed]
    C --> E
    D --> E
    E --> F[Hardware-mapped I/O]
    E --> G[Interrupt flags]
    E --> H[Memory-mapped registers]
```

## volatile vs const

```mermaid
flowchart TD
    A[Type Qualifiers] --> B[const]
    A --> C[volatile]
    B --> D[Read-only - compiler enforces]
    C --> E[Always re-read from memory]
    D --> F[Cannot modify]
    E --> G[Compiler must not cache]
    A --> H[const volatile]
    H --> I[Read-only but always from memory]
    H --> J[Hardware status registers]
```

## Implementation Checklist

- [ ] Parse `volatile` keyword
- [ ] Disable register caching for volatile reads
- [ ] Generate memory load/store for every volatile access
- [ ] Test: volatile read generates memory load every iteration

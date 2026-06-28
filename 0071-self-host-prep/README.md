# Lesson 0071: Self-Hosting Preparation

## Status: 📋 Planned | Phase: Self-Hosting | Effort: Hard

## Objective

Prepare compiler to compile itself.

## Self-Hosting Readiness

```mermaid
graph TD
    A[Audit Compiler Source] --> B{Feature Supported?}
    B -->|Yes| C[Mark Complete]
    B -->|No| D[Implement Feature]
    D --> E[Test Feature]
    E --> B
    C --> F[All Features Complete?]
    F -->|No| A
    F -->|Yes| G[Ready for Self-Hosting]
```

## Requirements

To compile itself, the compiler needs to support:
- All C features used in its own source code
- Preprocessor (for headers)
- Multi-file compilation
- All data types used (structs, enums, etc.)

## Implementation Checklist

- [ ] Audit compiler source for unsupported features
- [ ] Implement missing features
- [ ] Test: compile a simplified version of the compiler
- [ ] Bootstrap: use gcc to compile simplecc, then use simplecc to compile itself

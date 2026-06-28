# Lesson 0048: Inline Assembly

## Status: 📋 Planned | Phase: Float & Advanced | Effort: Medium (6-8h)

## Objective

Implement `asm("instruction")` for inline assembly.

## Inline Assembly Processing

```mermaid
flowchart TD
    A["asm(\"mov $42, %rax\")"] --> B[Parse asm statement]
    B --> C[Extract assembly string]
    C --> D[Parse operand constraints]
    D --> E[Emit raw assembly in output]
    E --> F[Map C variables to registers]
    F --> G[Generate prologue/epilogue]

    H["Operand modifiers"] --> I["= output"]
    H --> J["+ read/write"]
    H --> K["r register"]
    H --> L["m memory"]
```

## Implementation Checklist

- [ ] Parse `asm("...")` expression
- [ ] Emit raw assembly in output
- [ ] Support basic operand constraints
- [ ] Test: `asm("mov $42, %rax");` → return 42

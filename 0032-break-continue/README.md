# Lesson 0032: break/continue (Proper)

## Status: 📋 Planned | Phase: Control Flow | Effort: Easy (2-3h)

## Objective

Implement proper break/continue with loop context tracking.

## Break and Continue Flow

```mermaid
flowchart TD
    A["break"] --> B{Current loop?}
    B -->|Yes| C[Jump to loop exit label]
    B -->|No| D{Current switch?}
    D -->|Yes| E[Jump to switch end]
    D -->|No| F[Error: break outside loop]

    G["continue"] --> H{Current loop?}
    H -->|Yes| I[Jump to loop condition/update]
    H -->|No| J[Error: continue outside loop]

    K["while (cond)"] --> L[Emit loop_start label]
    L --> M[Emit loop body]
    M --> N[Emit loop_end label]

    O["break in while"] --> P["jmp loop_end"]
    Q["continue in while"] --> R["jmp loop_start"]

    S["for (init; cond; incr)"] --> T[Init]
    T --> U[Emit loop_start]
    U --> V[Emit body]
    V --> W[Incr]
    W --> U
    U --> X[Emit loop_end]
```

## Implementation Checklist

- [ ] Track current loop in codegen
- [ ] break → jump to loop exit label
- [ ] continue → jump to loop condition/update
- [ ] break in switch → jump to switch end
- [ ] Error on break/continue outside loop
- [ ] Test: nested loop break exits innermost only

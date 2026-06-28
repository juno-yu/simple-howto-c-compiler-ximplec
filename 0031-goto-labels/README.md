# Lesson 0031: goto and Labels

## Status: 📋 Planned | Phase: Control Flow | Effort: Medium (4-6h)

## Objective

Implement `goto label;` and `label: statement`.

## Goto and Labels Flow

```mermaid
flowchart TD
    A["goto label;"] --> B[Parse goto keyword]
    B --> C[Parse label identifier]
    C --> D[Emit jump instruction]

    E["label: stmt"] --> F[Parse label identifier]
    F --> G[Parse colon]
    G --> H[Emit label marker]
    H --> I[Parse statement]

    J["Forward goto"] --> K["goto end;"]
    K --> L["... skipped code ..."]
    L --> M["end: return 42;"]

    N["Backward goto"] --> O["loop:"]
    O --> P["... loop body ..."]
    P --> Q["if (cond) goto loop;"]
    Q --> O
```

## Implementation Checklist

- [ ] Parse `goto label;`
- [ ] Parse `label: statement`
- [ ] Forward and backward jumps
- [ ] Validate goto targets exist
- [ ] Test: `goto end; ... end: return 42;`

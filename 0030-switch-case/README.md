# Lesson 0030: switch/case/default

## Status: 📋 Planned | Phase: Control Flow | Effort: Medium (8-12h)

## Objective

Implement switch statement with case labels.

## Implementation Checklist

- [ ] Parse `switch(expr) { case val: ... default: ... }`
- [ ] Case values must be compile-time constants
- [ ] Implement if-else lowering (simple approach)
- [ ] Implement break inside switch cases
- [ ] Fall-through behavior between cases
- [ ] Test: basic switch with 3 cases + default

## Architecture

```mermaid
flowchart TD
    A["switch(expr)"] --> B["Evaluate expression"]
    B --> C{"Match case?"}
    C -->|case 1| D["Execute case 1 body"]
    C -->|case 2| E["Execute case 2 body"]
    C -->|case N| F["Execute case N body"]
    C -->|no match| G["Execute default body"]

    D --> H{"Has break?"}
    E --> H
    F --> H
    G --> H

    H -->|Yes| I["Exit switch"]
    H -->|No| J["Fall through to next case"]

    K["Lowering Strategy"] --> L["Convert to if-else chain"]
    L --> M["case 1: if (expr == 1) goto label1"]
    L --> N["case 2: if (expr == 2) goto label2"]
    L --> O["default: goto default_label"]

    P["Labels"] --> Q["label1: stmts; if break goto end"]
    P --> R["label2: stmts; if break goto end"]
    P --> S["default_label: stmts; goto end"]

    style C fill:#ffd,stroke:#333
    style K fill:#f9f,stroke:#333
```

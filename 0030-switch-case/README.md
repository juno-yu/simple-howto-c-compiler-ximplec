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

## Implementation Details

| Feature | File | Line(s) | Description |
|---------|------|---------|-------------|
| Lexer keywords | `src/lexer.cpp` | 25–27, 109 | `switch`, `case`, `default` tokens |
| AST nodes | `src/ast.h` | 92–94, 269–289 | `SwitchStmtNode`, `CaseLabelNode`, `DefaultLabelNode` |
| AST accept | `src/ast.cpp` | 15–17 | `accept()` methods |
| Parser entry | `src/parser.cpp` | 666–668 | Dispatches to `parse_switch_stmt()` |
| Parser method | `src/parser.cpp` | 816–858 | Parses `switch(expr) { case… default… }` |
| Codegen | `src/codegen.cpp` | 414–464 | If-else lowering with case comparison chain |
| Codegen helpers | `src/codegen.cpp` | 466–471 | Case/default label visitors (no-ops) |
| Codegen context | `src/codegen.h` | 97–98 | `current_case_label_`, `current_switch_end_` |

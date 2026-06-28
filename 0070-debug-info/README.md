# Lesson 0070: Debug Information (DWARF)

## Status: 📋 Planned | Phase: Optimization | Effort: Hard

## Objective

Generate DWARF debug info for gdb/lldb.

## Debug Info Generation

```mermaid
graph TD
    A[Source Code] --> B[Tokenizer]
    B --> C[Parser]
    C --> D[AST with Source Locations]
    D --> E[.debug_info Section]
    D --> F[.debug_line Section]
    D --> G[.debug_abbrev Section]
    E --> H[DWARF Output]
    F --> H
    G --> H
```

## Implementation Checklist

- [ ] Generate `.debug_info` section
- [ ] Generate `.debug_line` section (line numbers)
- [ ] Generate `.debug_abbrev` section
- [ ] Map source locations to addresses
- [ ] Describe types and variables
- [ ] Test: `gcc -g` produces debuggable binary

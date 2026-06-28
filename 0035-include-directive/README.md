# Lesson 0035: #include Directive

## Status: 📋 Planned | Phase: Preprocessor | Effort: Medium (8-12h)

## Objective

Implement `#include` for header file processing.

## Include Processing Flow

```mermaid
flowchart TD
    A["#include directive"] --> B{"Quote style?"}

    B -->|"\"file\""| C[Search local directory first]
    C --> D[Then search include paths]
    B -->|"<file>"| E[Search include paths only]

    D --> F{"File found?"}
    E --> F
    F -->|Yes| G[Read file contents]
    F -->|No| H[Error: file not found]

    G --> I{"Include guard present?"}
    I -->|"#ifndef GUARD"| J{Guard defined?}
    J -->|Yes| K[Skip - already included]
    J -->|No| L[Process file content]
    L --> M[Define guard macro]
    I -->|No| L

    L --> N[Recursively process directives]
    N --> O[Insert into token stream]

    P["Include guard pattern"] --> Q["#ifndef MYHEADER_H"]
    Q --> R["#define MYHEADER_H"]
    R --> S["... header content ..."]
    S --> T["#endif"]
```

## Implementation Checklist

- [ ] Parse `#include <file>` and `#include "file"`
- [ ] Search include paths
- [ ] Recursive include processing
- [ ] Include guard support (`#ifndef`/`#define`/`#endif`)
- [ ] Prevent infinite recursion
- [ ] Test: include a custom header with function declarations

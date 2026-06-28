# Lesson 3006: #embed (C23)

## Status: 📋 Planned | Standard: C23 | Effort: Medium

## Objective

Include binary data directly in source code.

## Syntax

```c
// Include file contents as comma-separated integers
const unsigned char icon[] = {
    #embed "icon.png"
};

// With fallback
const unsigned char data[] = {
    #embed "data.bin"
    // if file not found, use fallback:
    , 0x00, 0xFF, 0x01
};
```

## Comparison with GCC Extension

```c
// GCC (before C23)
#include "data.bin"  // includes as text, problematic

// C23 #embed
const char data[] = { #embed "data.bin" };  // proper binary include
```

## Implementation Checklist

- [ ] Parse `#embed "filename"` directive
- [ ] Read file as binary
- [ ] Generate comma-separated integer list
- [ ] Handle fallback if file not found
- [ ] Support include path search
- [ ] Test: embed small binary file
- [ ] Test: error on missing file without fallback

## Flow Diagram

```mermaid
flowchart TD
    A[Source: #embed "file.bin"] --> B[Preprocessor]
    B --> C{Directive Type?}
    C -->|#embed| D[Parse Embed Directive]
    C -->|Other| E[Normal Processing]
    D --> F[Extract Filename]
    F --> G[Search Include Path]
    G --> H{File Found?}
    H -->|Yes| I[Read Binary Contents]
    H -->|No| J{Fallback Provided?}
    J -->|Yes| K[Use Fallback Values]
    J -->|No| L[Error: File Not Found]
    I --> M[Convert to Byte Array]
    K --> M
    M --> N[Generate Comma-Separated List]
    N --> O[Insert into AST]
    O --> P[Codegen]
    P --> Q[Data Section in Assembly]
```

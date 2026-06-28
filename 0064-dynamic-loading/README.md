# Lesson 0064: Dynamic Loading

## Status: 📋 Planned | Phase: Stdlib Tier C | Effort: Hard

## Objective

Load shared libraries at runtime.

## Dynamic Loading Overview

```mermaid
flowchart TD
    A[Dynamic Loading] --> B[Library Loading]
    A --> C[Symbol Lookup]
    A --> D[Library Unloading]
    A --> E[Error Handling]

    B --> F[dlopen]
    C --> G[dlsym]
    D --> H[dlclose]
    E --> I[dlerror]

    F -->|"dlopen(filename, flags)"| J[Load library]
    G -->|"dlsym(handle, symbol)"| K[Find symbol]
    H -->|"dlclose(handle)"| L[Unload library]
    I -->|"dlerror()"| M[Get error message]
```

## Dynamic Loading Flow

```mermaid
sequenceDiagram
    participant App
    participant dlopen
    participant Library
    participant dlsym

    App->>dlopen: dlopen("libm.so", RTLD_LAZY)
    dlopen->>Library: Load library
    Library-->>dlopen: Handle
    dlopen-->>App: Return handle
    App->>dlsym: dlsym(handle, "sin")
    dlsym->>Library: Lookup symbol
    Library-->>dlsym: Function pointer
    dlsym-->>App: Return pointer
    App->>Library: Call sin()
```

## Functions

| Function | Complexity |
|----------|------------|
| `dlopen()` | Hard |
| `dlsym()` | Hard |
| `dlclose()` | Easy |
| `dlerror()` | Easy |

## Implementation Checklist

- [ ] Implement dlopen: parse ELF, load segments
- [ ] Implement dlsym: symbol lookup
- [ ] Implement dlclose: unload library
- [ ] Implement dlerror: error reporting
- [ ] Support RTLD_LAZY, RTLD_NOW
- [ ] Test: load libm, call sin()

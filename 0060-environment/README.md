# Lesson 0060: Environment Variables

## Status: 📋 Planned | Phase: Stdlib Tier B | Effort: Easy

## Objective

Access environment variables.

## Environment Variables Overview

```mermaid
flowchart TD
    A[Environment Variables] --> B[Access]
    A --> C[Modification]
    A --> D[Removal]

    B --> E[getenv]
    C --> F[setenv]
    D --> G[unsetenv]

    E -->|"getenv(name)"| H[Return value or NULL]
    F -->|"setenv(name, value, overwrite)"| I[Add/modify entry]
    G -->|"unsetenv(name)"| J[Remove entry]
```

## Environment Variable Flow

```mermaid
flowchart LR
    A[Process] --> B[environ array]
    B --> C["KEY=VALUE pairs"]
    C --> D[HOME=/home/user]
    C --> E[PATH=/usr/bin]
    C --> F[...]
    
    G["getenv(\"HOME\")"] --> H{Found?}
    H -->|Yes| I[Return value]
    H -->|No| J[Return NULL]
```

## Functions

| Function | Complexity |
|----------|------------|
| `getenv(name)` | Easy |
| `setenv(name, value, overwrite)` | Medium |
| `unsetenv(name)` | Medium |

## Implementation Checklist

- [ ] Access `environ` global variable
- [ ] Implement getenv: search environment array
- [ ] Implement setenv: add/modify environment entry
- [ ] Implement unsetenv: remove environment entry
- [ ] Test: `getenv("HOME")` returns home directory

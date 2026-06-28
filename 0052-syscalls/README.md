# Lesson 0052: System Calls

## Status: 📋 Planned | Phase: Stdlib Tier A | Effort: Medium (4-6h)

## Objective

Implement Linux syscall interface for I/O and memory.

## Syscall Interface

```mermaid
flowchart LR
    A[C Code] --> B[Load syscall number into %rax]
    B --> C[Load args into %rdi, %rsi, %rdx, %r10, %r8, %r9]
    C --> D[Execute syscall instruction]
    D --> E[Return value in %rax]
    E --> F{Check for error}
    F -->|rax >= 0| G[Success]
    F -->|rax < 0| H[Error - errno]
```

## Syscall Numbers (x86-64 Linux)

```mermaid
flowchart TD
    A[Linux Syscalls] --> B[0 - read]
    A --> C[1 - write]
    A --> D[2 - open]
    A --> E[3 - close]
    A --> F[9 - mmap]
    A --> G[12 - brk]
    A --> H[57 - fork]
    A --> I[59 - execve]
    A --> J[60 - exit]

    C --> K[write(fd, buf, len)]
    K --> L["%rax = 1, %rdi = fd, %rsi = buf, %rdx = len"]

    J --> M[exit(code)]
    M --> N["%rax = 60, %rdi = code"]
```

## Implementation Checklist

- [ ] `syscall` instruction wrapper
- [ ] `write(fd, buf, len)` → syscall 1
- [ ] `read(fd, buf, len)` → syscall 0
- [ ] `brk(addr)` → syscall 12 (for malloc)
- [ ] `exit(code)` → syscall 60
- [ ] Test: write "hello" to stdout via syscall

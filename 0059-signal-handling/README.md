# Lesson 0059: Signal Handling

## Status: 📋 Planned | Phase: Stdlib Tier B | Effort: Medium

## Objective

Implement signal handling functions.

## Signal Handling Overview

```mermaid
flowchart TD
    A[Signal Handling] --> B[Registration]
    A --> C[Generation]
    A --> D[Types]

    B --> E[signal]
    B --> F[sigaction]

    C --> G[kill]
    C --> H[raise]

    D --> I[SIGINT]
    D --> J[SIGTERM]
    D --> K[SIGCHLD]
    D --> L[SIGSEGV]

    E -->|"signal(sig, handler)"| M[Register handler]
    F -->|"sigaction(sig, act, oldact)"| N[Advanced registration]
    G -->|"kill(pid, sig)"| O[Send signal to process]
    H -->|"raise(sig)"| P[Send signal to self]
```

## Signal Flow

```mermaid
flowchart LR
    A[Process A] -->|"kill(pid, SIGINT)"| B[Kernel]
    B -->|"Deliver signal"| C[Process B]
    C --> D{Handler?}
    D -->|Yes| E[Run handler]
    D -->|No| F[Default action]
    E --> G[Return to execution]
```

## Functions

| Function | Complexity |
|----------|------------|
| `signal(sig, handler)` | Medium |
| `sigaction(sig, act, oldact)` | Medium |
| `kill(pid, sig)` | Easy |
| `raise(sig)` | Easy |

## Implementation Checklist

- [ ] Implement signal() via rt_sigaction syscall
- [ ] Implement sigaction() wrapper
- [ ] Implement kill() via kill syscall
- [ ] Define signal handler types
- [ ] Support SIGINT, SIGTERM, SIGCHLD
- [ ] Test: install SIGINT handler, send signal

## Implementation Details

The compiler handles signal handling through function pointer parsing and standard function call code generation.

| Component | Source File | Lines | Description |
|-----------|-------------|-------|-------------|
| Function pointer parsing | `src/parser.cpp` | 382–413 | Parses `void (*handler)(int)` declarations for signal handler parameters |
| Function call codegen | `src/codegen.cpp` | 838–853 | Generates `call` instructions for signal(), sigaction(), kill(), raise() |
| System V ABI registers | `src/codegen.cpp` | 267–268 | Maps first 6 args to `%rdi, %rsi, %rdx, %rcx, %r8, %r9` |
| Function prologue | `src/codegen.cpp` | 262–264 | Emits `push %rbp; mov %rsp, %rbp` for handler frames |
| Parameter passing | `src/codegen.cpp` | 288–291 | Stores parameters from registers to stack slots |

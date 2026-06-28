# Lesson 0057: Process Control

## Status: 📋 Planned | Phase: Stdlib Tier B | Effort: Medium (8-12h)

## Objective

Implement fork, exec, wait for process management.

## Process Control Overview

```mermaid
flowchart TD
    A[Process Control] --> B[Creation]
    A --> C[Execution]
    A --> D[Termination]
    A --> E[Information]

    B --> F[fork]
    C --> G[execvp]
    C --> H[execve]
    D --> I[waitpid]
    D --> J[exit]
    E --> K[getpid]

    F -->|"fork()"| L[Create child process]
    G -->|"execvp(cmd, args)"| M[Execute program]
    I -->|"waitpid(pid, status, opts)"| N[Wait for child]
    K -->|"getpid()"| O[Get process ID]
```

## Fork and Exec Flow

```mermaid
flowchart TD
    A[Parent Process] --> B[fork]
    B --> C{Child?}
    C -->|Yes| D[execvp]
    C -->|No| E[waitpid]
    D --> F[Run new program]
    E --> G[Wait for child]
    F --> H[Child exits]
    G --> I[Parent continues]
```

## Functions

| Function | Complexity |
|----------|------------|
| `fork()` | Medium |
| `execvp(cmd, args)` | Medium |
| `waitpid(pid, status, opts)` | Medium |
| `getpid()` | Easy |
| `exit(status)` | Trivial |

## Implementation Checklist

- [ ] Implement fork via syscall 57
- [ ] Implement execve via syscall 59
- [ ] Implement wait4 via syscall 61
- [ ] Implement getpid via syscall 39
- [ ] Test: fork and exec ls

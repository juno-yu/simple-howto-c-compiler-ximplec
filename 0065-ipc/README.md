# Lesson 0065: Inter-Process Communication

## Status: 📋 Planned | Phase: Stdlib Tier C | Effort: Hard

## Objective

Pipes, shared memory, message queues.

## IPC Overview

```mermaid
flowchart TD
    A[IPC] --> B[Pipes]
    A --> C[Shared Memory]
    A --> D[Named Pipes]

    B --> E[pipe]
    C --> F[shmget]
    C --> G[shmat]
    C --> H[shmdt]
    C --> I[shmctl]
    D --> J[mkfifo]

    E -->|"pipe(fds)"| K[Create anonymous pipe]
    F -->|"shmget(key, size, flags)"| L[Get shared memory]
    G -->|"shmat(id, addr, flags)"| M[Attach shared memory]
    H -->|"shmdt(addr)"| N[Detach shared memory]
    J -->|"mkfifo(path, mode)"| O[Create named pipe]
```

## Pipe Communication

```mermaid
sequenceDiagram
    participant Parent
    participant Pipe
    participant Child

    Parent->>Pipe: pipe(fds)
    Parent->>Child: fork()
    Note over Parent: Write to pipe
    Parent->>Pipe: write(fds[1], data)
    Note over Child: Read from pipe
    Child->>Pipe: read(fds[0], buf)
```

## Shared Memory

```mermaid
flowchart LR
    A[Process A] --> B[shmget]
    B --> C[shmat]
    C --> D[Shared Segment]
    E[Process F] --> F[shmget]
    F --> G[shmat]
    G --> D
    D --> H[Read/Write]
```

## Functions

| Function | Complexity |
|----------|------------|
| `pipe()` | Medium |
| `mkfifo()` | Medium |
| `shmget/shmat/shmdt/shmctl` | Hard |
| `msgget/msgsnd/msgrcv/msgctl` | Hard |

## Implementation Checklist

- [ ] Implement pipe via pipe2 syscall
- [ ] Implement FIFO via mkfifo
- [ ] Implement shared memory via shmget/shmat
- [ ] Implement message queues
- [ ] Test: parent-child communication via pipe

## Implementation Details

Inter-process communication is supported through extern function declarations and the standard function call code generation.

| Component | Source File | Lines | Description |
|-----------|-------------|-------|-------------|
| Function declaration parsing | `src/parser.cpp` | 233–250 | Parses `int pipe(int pipefd[2])` and shared memory declarations |
| Array parameter handling | `src/parser.cpp` | 148–170 | Handles array parameters `int pipefd[2]` in function signatures |
| Function call codegen | `src/codegen.cpp` | 838–853 | Generates `call pipe` with array arg passed as pointer in `%rdi` |
| Array indexing codegen | `src/codegen.cpp` | 856–895 | Generates `mov %rax, N(%rdi)` for `pipefd[0]`/`pipefd[1]` access |
| Block statement codegen | `src/codegen.cpp` | 489–492 | Sequential execution of pipe/fork/read/write calls |

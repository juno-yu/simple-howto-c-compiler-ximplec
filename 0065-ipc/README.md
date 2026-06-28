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

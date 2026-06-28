# Lesson 1010: _Thread_local (C11)

## Status: ✅ Complete | Standard: C11 | Effort: Medium

## Objective

Thread-local storage for per-thread data.

## Syntax

```c
_thread_local int counter = 0;  // each thread has its own copy
__thread int tls_var;           // GCC extension (same)
```

## Semantics

```mermaid
graph TD
    A["_Thread_local int x = 0"] --> B[Thread 1: x = 0]
    A --> C[Thread 2: x = 0]
    A --> D[Thread 3: x = 0]
    B --> E["x++ → Thread 1: x = 1"]
    C --> F["x++ → Thread 2: x = 1"]
    D --> G["x++ → Thread 3: x = 1"]
```

## Implementation

- On Linux: uses `__thread` GCC extension or `thread_local` (C23)
- Storage in TLS segment of ELF
- Accessed via FS/GS segment register offset

## Implementation Checklist

- [ ] Parse `_Thread_local` keyword
- [ ] Allocate in TLS segment (.tdata/.tbss)
- [ ] Access via FS-relative addressing
- [ ] Initialize per-thread on thread creation
- [ ] Destroy on thread exit
- [ ] Test: increment counter in 3 threads, verify isolation

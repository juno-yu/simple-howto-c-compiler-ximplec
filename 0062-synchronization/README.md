# Lesson 0062: Synchronization Primitives

## Status: 📋 Planned | Phase: Stdlib Tier C | Effort: Hard

## Objective

Mutexes, condition variables, semaphores.

## Synchronization Overview

```mermaid
flowchart TD
    A[Synchronization] --> B[Mutex]
    A --> C[Semaphore]

    B --> D[pthread_mutex_init]
    B --> E[pthread_mutex_lock]
    B --> F[pthread_mutex_unlock]
    B --> G[pthread_mutex_destroy]

    C --> H[sem_init]
    C --> I[sem_wait]
    C --> J[sem_post]
    C --> K[sem_destroy]

    D -->|"pthread_mutex_init(mtx, attr)"| L[Initialize mutex]
    E -->|"pthread_mutex_lock(mtx)"| M[Lock mutex]
    F -->|"pthread_mutex_unlock(mtx)"| N[Unlock mutex]
    H -->|"sem_init(sem, shared, value)"| O[Initialize semaphore]
    I -->|"sem_wait(sem)"| P[Decrement semaphore]
    J -->|"sem_post(sem)"| Q[Increment semaphore]
```

## Mutex vs Semaphore

```mermaid
flowchart LR
    subgraph Mutex
        A[Only one thread]
        B[Can lock at a time]
    end
    
    subgraph Semaphore
        C[Multiple threads]
        D[Can access up to count]
    end
```

## Functions

| Function | Complexity |
|----------|------------|
| `pthread_mutex_init/lock/unlock/destroy` | Medium |
| `pthread_cond_init/wait/signal/broadcast/destroy` | Hard |
| `sem_init/wait/post/destroy` | Medium |

## Implementation Checklist

- [ ] Implement mutex via futex
- [ ] Implement condition variable via futex
- [ ] Implement semaphore via futex
- [ ] Spinlock implementation
- [ ] Read-write locks
- [ ] Test: producer-consumer pattern

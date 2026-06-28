# Lesson 0061: Thread Support (POSIX)

## Status: 📋 Planned | Phase: Stdlib Tier C | Effort: Hard

## Objective

POSIX thread (pthreads) implementation.

## Functions

| Function | Complexity |
|----------|------------|
| `pthread_create()` | Hard |
| `pthread_join()` | Medium |
| `pthread_detach()` | Easy |
| `pthread_exit()` | Easy |
| `pthread_self()` | Trivial |

## Implementation Checklist

- [ ] Implement pthread_create via clone syscall
- [ ] Implement pthread_join via futex wait
- [ ] Implement pthread_detach
- [ ] Thread stack allocation
- [ ] Thread-local storage
- [ ] Test: create thread, join, verify result

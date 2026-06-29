# Lesson 1006: Threads (C11)

## Status: ⚠️ Skeleton | Standard: C11 | Effort: Hard

## Objective

Recognize the C11 threading API surface.

## API

```c
#include <threads.h>

thrd_t thread;
thrd_create(&thread, func, arg);
thrd_join(thread, &result);
thrd_detach(thread);

mtx_t mutex;
mtx_init(&mutex, mtx_plain);
mtx_lock(&mutex);
mtx_unlock(&mutex);
mtx_destroy(&mutex);

cnd_t cond;
cnd_init(&cond);
cnd_wait(&cond, &mutex);
cnd_signal(&cond);
cnd_broadcast(&cond);
cnd_destroy(&cond);
```

## How It Works

simplecc ships **no** `<threads.h>` header in `lib/`, no thread-storage lowering, and no wrapper for `pthread_create` / `pthread_join`. The bundled example in `1006-c11-threads/src/example.c` is a stub that returns the literal `42` without ever calling `thrd_create`.

## Implementation (intended)

Typically wraps POSIX threads (pthreads) or Win32 threads. None of that is present in the current source.

## What Works

| Feature | Status |
|---------|--------|
| `thrd_create` | ❌ No header |
| `thrd_join` | ❌ |
| `thrd_detach` | ❌ |
| `mtx_init/lock/unlock/destroy` | ❌ |
| `cnd_init/wait/signal/broadcast/destroy` | ❌ |
| `call_once` | ❌ |
| `tss_*` thread-local storage | ❌ |

## Source Code References

| Component | File | Status |
|-----------|------|--------|
| `<threads.h>` | `lib/` | ❌ Not present |
| Codegen | `src/codegen.cpp` | No thread-aware path |

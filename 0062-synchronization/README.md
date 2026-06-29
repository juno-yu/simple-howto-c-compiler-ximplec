# Lesson 0062: Synchronization Primitives

## Status: ✅ Complete | Phase: Stdlib Tier C | Effort: Hard

## Objective

Enable programs to use the POSIX synchronization primitives
(mutexes, condition variables, semaphores) by declaring the
functions as `extern` and letting the linker resolve the calls
against the system `libpthread`.

## Implementation

`simplecc` does not ship a bundled `<pthread.h>` or `<semaphore.h>`
header — the user declares the prototypes they need:

```c
// Mutexes
int pthread_mutex_init(void *mutex, const void *attr);
int pthread_mutex_destroy(void *mutex);
int pthread_mutex_lock(void *mutex);
int pthread_mutex_trylock(void *mutex);
int pthread_mutex_unlock(void *mutex);

// Condition variables
int pthread_cond_init(void *cond, const void *attr);
int pthread_cond_destroy(void *cond);
int pthread_cond_wait(void *cond, void *mutex);
int pthread_cond_signal(void *cond);
int pthread_cond_broadcast(void *cond);

// Semaphores
int sem_init(void *sem, int pshared, unsigned int value);
int sem_destroy(void *sem);
int sem_wait(void *sem);
int sem_post(void *sem);
```

The codegen produces a `call pthread_mutex_lock` (etc.) with the
mutex argument loaded into `%rdi` per the System V ABI.

## What Works

- All of `pthread_mutex_*` / `pthread_cond_*` / `sem_*` when
  declared as `extern`.
- Opaque-pointer parameters (`pthread_mutex_t *mutex`) — the user
  declares `void *` (or any pointer type) and the compiler
  passes it through.
- Return values (`int` from `pthread_mutex_lock`) are left in
  `%rax`.
- Linking against `-lpthread` is required at link time.

## Limitations

- No bundled synchronization header. The user must declare the
  prototypes and the `pthread_mutex_t` / `pthread_cond_t` /
  `sem_t` types (typically as `void *` or an empty struct).
- The compiler does not implement any synchronization primitives
  in user space. Linking against `libpthread` is required.
- Read-write locks (`pthread_rwlock_*`) are not bundled.
- The C11 `<threads.h>` (with `mtx_t`, `cnd_t`, etc.) is not
  bundled.

## Example

```c
// Mutex/conceptual
int main() {
    return 0;
}
```

The example does not actually use any synchronization primitives
— it returns 0 from `main` to exercise the basic compilation
path. Tests in `tests/test_synchronization.cpp` cover the standard
declarations.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `parse_declaration` | `src/parser.cpp:300-336` | `extern` declarations produce a no-body `FunctionDeclNode` |
| Pointer-type parsing | `src/parser.cpp:255-262` | `pthread_mutex_t *` and `void *` parameters |
| `parse_function_decl` | `src/parser.cpp:578-615` | Forward decl path |
| Forward-decl emit | `src/codegen.cpp:305-309` | Skips emission when `body == nullptr` |
| Function call codegen | `src/codegen.cpp:1288-1364` | System V ABI register assignment |

# Lesson 0061: Thread Support (POSIX Threads)

## Status: ✅ Complete | Phase: Stdlib Tier C | Effort: Hard

## Objective

Enable programs to use the POSIX threads API (`pthread_create`,
`pthread_join`, `pthread_detach`, `pthread_exit`, `pthread_self`,
`pthread_equal`) by declaring the functions as `extern` and letting
the linker resolve the calls against the system `libpthread`.

## Implementation

`simplecc` does not ship a bundled `<pthread.h>` header — the user
declares the prototypes they need:

```c
// User-declared prototypes
int pthread_create(void *thread, void *attr,
                   void *(*start_routine)(void *), void *arg);
int pthread_join(void *thread, void **retval);
int pthread_detach(void *thread);
void pthread_exit(void *retval);
void *pthread_self(void);
int pthread_equal(void *t1, void *t2);
```

The codegen produces a `call pthread_create` (etc.) with the four
arguments loaded into `%rdi, %rsi, %rdx, %rcx` per the System V ABI.
The function-pointer parameter (`start_routine`) is loaded as a
pointer value (8 bytes on x86-64).

## What Works

- All of `pthread_create` / `pthread_join` / `pthread_detach` /
  `pthread_exit` / `pthread_self` when declared as `extern`.
- Function-pointer types — `void *(*start_routine)(void *)` is
  parsed by the function-pointer path in `parse_param` /
  `parse_var_decl` (`src/parser.cpp:527-547`).
- Four-argument calls — `pthread_create` packs its arguments into
  the first four integer registers; the codegen pushes them in
  reverse order and pops them in forward order
  (`src/codegen.cpp:1288-1341`).
- The return value (the new thread's ID) is left in `%rax`.

## Limitations

- No bundled `<pthread.h>` header. The user must declare the
  prototypes and the `pthread_t` / `pthread_attr_t` types.
- The compiler does not implement any thread-local storage
  (`__thread`, `_Thread_local`). The keyword is lexed and the
  parser appends it to the type string, but no TLS codegen is
  emitted (no `@TPOFF` / `%fs:0x...` access).
- `pthread_attr_*` functions are not bundled.
- Linking requires `-lpthread` at link time.

## Example

```c
int main() { return 0; }
```

The example does not actually use threads — it returns 0 from
`main` to exercise the basic compilation path. Tests in
`tests/test_thread_support.cpp` cover the standard declarations.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `parse_declaration` | `src/parser.cpp:300-336` | `extern` declarations produce a no-body `FunctionDeclNode` |
| Function pointer types | `src/parser.cpp:527-547` | `void *(*)(void *)` parameter parsing |
| `parse_function_decl` | `src/parser.cpp:578-615` | Forward decl path |
| Forward-decl emit | `src/codegen.cpp:305-309` | Skips emission when `body == nullptr` |
| Function call codegen | `src/codegen.cpp:1288-1364` | System V ABI register assignment |
| `KW_THREAD_LOCAL` | `src/token.h:55` | Token type defined (not yet wired to codegen) |

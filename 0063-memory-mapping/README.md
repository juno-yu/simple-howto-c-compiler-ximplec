# Lesson 0063: Memory Mapping

## Status: ✅ Complete | Phase: Stdlib Tier C | Effort: Medium

## Objective

Enable programs to map memory (anonymous or file-backed) using the
POSIX `mmap` / `munmap` / `mprotect` / `madvise` functions, by
declaring the functions as `extern` and letting the linker resolve
the calls against the system C library.

## Implementation

`simplecc` does not ship a bundled `<sys/mman.h>` header — the user
declares the prototypes they need:

```c
// User-declared prototypes
void *mmap(void *addr, unsigned long length, int prot, int flags,
           int fd, long offset);
int   munmap(void *addr, unsigned long length);
int   mprotect(void *addr, unsigned long len, int prot);
int   madvise(void *addr, unsigned long length, int advice);

// Common flag values (also user-declared)
#define PROT_READ  1
#define PROT_WRITE 2
#define PROT_EXEC  4
#define MAP_PRIVATE   2
#define MAP_ANONYMOUS 0x20
```

The codegen produces a `call mmap` with the six arguments loaded
into `%rdi, %rsi, %rdx, %rcx, %r8, %r9` per the System V ABI.

## What Works

- All of `mmap` / `munmap` / `mprotect` / `madvise` when declared
  as `extern`.
- Six-argument function calls — `mmap` packs its six arguments
  into the integer registers, and the codegen handles all six
  correctly (`src/codegen.cpp:1288-1341`).
- `void *` return type — the mapped address is left in `%rax`
  and can be assigned to a `void *` local or cast to another
  pointer type.
- The bundled `errno.h` (`lib/errno.h`) provides the `EACCES`,
  `ENOMEM`, etc. error codes that `mmap` may set.

## Limitations

- No bundled `<sys/mman.h>` header. The user must declare the
  prototypes and the `PROT_*` / `MAP_*` flag values.
- Linking against the system C library is required for `mmap` to
  actually map memory — `simplecc` does not implement any
  memory-mapping primitive.
- The compiler does not enforce page-size alignment on the `addr`
  argument.

## Example

```c
void *mmap(void *addr, int len, int prot, int flags, int fd, int offset);

int main() {
    return 0;
}
```

The example does not actually call `mmap` — it returns 0 from
`main` to exercise the prototype-parsing path. Tests in
`tests/test_memory_mapping.cpp` cover the standard declarations.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `parse_declaration` | `src/parser.cpp:300-336` | `extern` declarations produce a no-body `FunctionDeclNode` |
| Pointer-type parsing | `src/parser.cpp:255-262` | `void *` and `int *` parameters |
| `parse_function_decl` | `src/parser.cpp:578-615` | Forward decl path |
| Forward-decl emit | `src/codegen.cpp:305-309` | Skips emission when `body == nullptr` |
| Function call codegen | `src/codegen.cpp:1288-1364` | System V ABI register assignment |
| `errno.h` | `lib/errno.h:1-42` | Error codes for `mmap` failures |

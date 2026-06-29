# Lesson 0055: Memory Allocation

## Status: ✅ Complete | Phase: Stdlib Tier A | Effort: Medium

## Objective

Enable programs to call the standard C memory-allocation functions
(`malloc`, `free`, `calloc`, `realloc`, `aligned_alloc`) by declaring
them as `extern` and letting the linker resolve the calls against the
system C library.

## Implementation

`simplecc` ships a minimal bundled header in `lib/stdlib.h`:

```c
// lib/stdlib.h
#pragma once

#define NULL ((void*)0)
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

void *malloc(int size);
void *calloc(int nmemb, int size);
void *realloc(void *ptr, int size);
void  free(void *ptr);
void  exit(int status);
void  abort();

int  atoi(const char *s);
long atol(const char *s);

int  abs(int x);
long labs(long x);

int  rand();
void srand(unsigned int seed);
```

When a user writes `#include <stdlib.h>` the preprocessor reads
`lib/stdlib.h` and substitutes the declarations.

The compiler does not implement a bump allocator, a `brk`-based
allocator, or any other in-compiler allocator. Programs that call
`malloc`/`free` are linked against the system C library, which provides
a fully functional `ptmalloc2` (or equivalent) implementation.

## What Works

- Declaring any of `malloc`/`free`/`calloc`/`realloc` as `extern` and
  calling it. The codegen produces a `call malloc` that the linker
  resolves at link time.
- `sizeof(int)` / `sizeof(*p)` as a `malloc` argument — the codegen's
  `visit(SizeofExprNode)` (`src/codegen.cpp:1120-1140`) emits the byte
  size as an immediate.
- Pointer-store through a `malloc`-returned pointer:
  `*p = 42;` lowers to `mov $42, (%rax)` (`src/codegen.cpp:888-895`).

## Limitations

- No in-compiler allocator. Linking against the system libc is required.
- The `size` parameter is declared as `int`, not `size_t`. Programs
  that allocate more than 2 GiB will not type-check correctly.
- The bundled `stdlib.h` does not declare `aligned_alloc`, `posix_memalign`,
  or `memalign` — those can be added by the user.

## Example

```c
void *malloc(int size);
void  free(void *p);

int main() {
    int *p = malloc(sizeof(int));
    *p = 42;
    free(p);
    return 0;
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Bundled header | `lib/stdlib.h:1-24` | Declarations of `malloc`/`free`/`calloc`/etc. |
| `parse_param` | `src/parser.cpp:937-942` | Recognises `...` in function declarations |
| Pointer-type parsing | `src/parser.cpp:255-262` | `*` token appended to the type string |
| `sizeof` codegen | `src/codegen.cpp:1120-1140` | Emits the byte size as an immediate |
| Deref-store codegen | `src/codegen.cpp:888-895` | `mov $42, (%rax)` for `*p = 42;` |
| Function call codegen | `src/codegen.cpp:1288-1364` | `call malloc` with `size` in `%rdi` |

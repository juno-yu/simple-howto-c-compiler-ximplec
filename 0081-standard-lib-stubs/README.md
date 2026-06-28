# Lesson 0081: Standard Library Stubs

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Easy

## Objective

Document and demonstrate the convention for using standard library functions in programs compiled by `simplecc`. Standard library headers are **not bundled** with this project; the compiled object code links against the **system** libc/libm at link time.

## How It Works

`simplecc` produces x86-64 GAS assembly that calls functions by name (e.g., `printf`, `malloc`, `strlen`). At link time, the system `gcc`/`ld` resolves these names against the system's C standard library. The user is responsible for providing declarations (via their own header files) for any function they call.

## Bundled Stubs

The project does not ship a standard library, but the following function families are commonly used and are linked at build time:

| Header | Functions | Link With |
|--------|-----------|-----------|
| `<stdio.h>` | `printf`, `scanf`, `fopen`, `fclose`, `fread`, `fwrite` | `-lc` (libc) |
| `<stdlib.h>` | `malloc`, `free`, `calloc`, `realloc`, `exit`, `atoi` | `-lc` |
| `<string.h>` | `strlen`, `strcpy`, `strcmp`, `memcpy`, `memset` | `-lc` |
| `<math.h>` | `sin`, `cos`, `sqrt`, `pow`, `exp`, `log` | `-lm` (libm) |
| `<ctype.h>` | `isdigit`, `isalpha`, `toupper`, `tolower` | `-lc` |
| `<stdint.h>` | `int32_t`, `uint64_t`, `INT_MAX`, etc. | `-lc` |
| `<threads.h>` | `thrd_create`, `mtx_lock`, `cnd_wait` | `-lpthread` |
| `<stdatomic.h>` | `atomic_int`, `atomic_load`, `atomic_store` | `-latomic` (often) |

## Limitations

- **No bundled headers.** `#include <stdio.h>` fails because the file is not in the include path. Users must write their own minimal declarations.
- **No standard library test suite** — each program is responsible for declaring the functions it uses.
- **No freestanding mode** — assume hosted environment.

## Example

A program that uses standard library functions:

```c
// User must declare these manually:
int printf(const char *fmt, ...);
void *malloc(long size);
void free(void *ptr);
int strlen(const char *s);
void *memcpy(void *dst, const void *src, long n);

int main() {
    char *buf = (char *)malloc(64);
    int n = strlen("hello");
    memcpy(buf, "hello", n + 1);
    printf("len=%d buf=%s\n", n, buf);
    free(buf);
    return 0;
}
```

## Building Programs That Use stdlib

```bash
./build/simplecc -S program.c -o program.s
gcc -o program program.s         # links against libc automatically
./program
```

For math: `gcc -o program program.s -lm`.
For threads: `gcc -o program program.s -lpthread`.

## Source Code References

- No special compiler support — the program simply references external symbols that the linker resolves.

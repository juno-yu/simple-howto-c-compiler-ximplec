# Lesson 0081: Standard Library Stubs

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Easy

## Objective

Document and demonstrate the convention for using standard library
functions in programs compiled by `simplecc`. A minimal set of
bundled headers lives in `lib/` and is automatically searched by
the preprocessor for `#include <...>` directives. Anything not in
`lib/` is linked against the system C library at link time.

## How It Works

`simplecc` produces x86-64 GAS assembly that calls functions by name
(e.g. `printf`, `malloc`, `strlen`). At link time, the system
`gcc`/`ld` resolves these names against the system's C standard
library. The user is responsible for providing declarations for any
function they call — either by `#include`-ing a bundled header
(from `lib/`) or by writing their own.

## Bundled Headers (`lib/`)

The preprocessor searches the `lib/` directory for any
`#include <...>` (and quote-includes as a fallback). The bundled
headers are minimal — they declare the common functions and a few
constants but do not implement anything:

| Header | Contents | Link With |
|--------|----------|-----------|
| `lib/assert.h` | `assert(expr)` macro | `-lc` |
| `lib/ctype.h` | `isdigit`/`isalpha`/`isspace`/`toupper`/`tolower` | `-lc` |
| `lib/errno.h` | `errno` decl + `EDOM`/`ERANGE`/`EINVAL`/etc. | `-lc` |
| `lib/math.h` | `sin`/`cos`/`sqrt`/`pow`/`exp`/`log`/etc. | `-lm` |
| `lib/stdalign.h` | `alignas` / `alignof` macros | header-only |
| `lib/stdbool.h` | `true` / `false` macros | header-only |
| `lib/stddef.h` | `NULL` and `offsetof` macro | header-only |
| `lib/stdint.h` | `INT8_MIN`/`INT32_MAX`/`UINT64_MAX`/etc. | header-only |
| `lib/stdio.h` | `printf`/`fopen`/`fread`/`fwrite`/`FILE`/etc. | `-lc` |
| `lib/stdlib.h` | `malloc`/`free`/`calloc`/`realloc`/`exit`/etc. | `-lc` |
| `lib/string.h` | `strlen`/`strcmp`/`strcpy`/`memcpy`/`memset`/etc. | `-lc` |

The preprocessor reads the header directly and inlines it as text
into the output, so there is no separate "header" compilation pass.
See `src/preprocessor.cpp:95-130` for the `#include` resolution
logic.

## Example

A program that uses standard library functions via the bundled
headers:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *buf = malloc(64);
    int n = strlen("hello");
    memcpy(buf, "hello", n + 1);
    printf("len=%d buf=%s\n", n, buf);
    free(buf);
    exit(0);
}
```

The bundled `lib/stdio.h` declares `printf`, `lib/stdlib.h` declares
`malloc`/`free`/`exit`, and `lib/string.h` declares `strlen`/`memcpy`.
The actual implementations come from the system C library at link
time.

## Building Programs That Use stdlib

```sh
./build/simplecc -S program.c -o program.s
gcc -o program program.s              # links against libc automatically
./program
```

For math: `gcc -o program program.s -lm`.
For threads: `gcc -o program program.s -lpthread`.

## Limitations

- The bundled headers are minimal — they do not declare every
  function in the corresponding standard. Functions not declared
  must be hand-declared as `extern` in the source.
- **No freestanding mode** — assume hosted environment with libc
  available at link time.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `#include` resolution | `src/preprocessor.cpp:95-130` | Looks up `lib/<name>` for angle-bracket and quote includes |
| Bundled headers | `lib/*.h` | Declarations for the common stdlib functions |
| Function call codegen | `src/codegen.cpp:1288-1364` | Emits `call` instructions for `extern` declarations |
| Forward-decl skip | `src/codegen.cpp:305-309` | Skips emission for `extern` declarations |

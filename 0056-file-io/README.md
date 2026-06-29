# Lesson 0056: File I/O

## Status: ✅ Complete | Phase: Stdlib Tier B | Effort: Medium

## Objective

Enable programs to perform file I/O using the standard C `FILE*` API
(`fopen`/`fclose`/`fread`/`fwrite`/`fgets`/`fputs`) and the lower-level
POSIX `open`/`close`/`read`/`write` API, by declaring the functions as
`extern` and letting the linker resolve the calls against the system
C library.

## Implementation

`simplecc` ships a minimal bundled header in `lib/stdio.h` that
declares the `FILE`-based API:

```c
// lib/stdio.h (excerpt)
#pragma once

typedef struct FILE FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

FILE *fopen(const char *pathname, const char *mode);
int   fclose(FILE *stream);
int   fread(void *ptr, int size, int nmemb, FILE *stream);
int   fwrite(const void *ptr, int size, int nmemb, FILE *stream);
char *fgets(char *s, int size, FILE *stream);
int   fputs(const char *s, FILE *stream);
int   fgetc(FILE *stream);
int   fputc(int c, FILE *stream);
int   feof(FILE *stream);
int   ferror(FILE *stream);
int   fflush(FILE *stream);
int   fseek(FILE *stream, long offset, int whence);
long  ftell(FILE *stream);
void  rewind(FILE *stream);
```

The `FILE` type is declared as `typedef struct FILE FILE;` — its
layout is never defined, so the user can only pass `FILE *` values
around. There is no bundled declaration of the POSIX `open`/`read`/
`write`/`close` functions; the user must declare them as `extern`
themselves.

## What Works

- Declaring any `FILE *` API function as `extern` and calling it.
- Passing `void *` / `FILE *` arguments — the compiler treats pointer
  types uniformly and emits `lea <label>(%rip), %rax` for string
  literals, `mov <offset>(%rbp), %rax` for local arrays, and the
  address-of operator for `&buf`.
- Multi-argument function calls (`fopen(path, mode)` lowers to
  `lea <str>(%rip), %rdi; lea <str>(%rip), %rsi; call fopen`).

## Limitations

- No bundled declaration of the POSIX `open`/`read`/`write`/`close`
  functions. The user must declare them.
- The opaque `FILE` structure is never defined; the user cannot
  access `FILE` fields directly (which is normal C).
- The bundled `stdio.h` does not declare `fscanf` or `vfprintf` —
  these can be added by the user.

## Example

```c
void *fopen(char *path, char *mode);
int   fclose(void *f);

int main() {
    void *f = fopen("test.txt", "r");
    if (f) fclose(f);
    return 0;
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Bundled header | `lib/stdio.h:6-37` | `FILE` typedef and the `FILE*` API |
| Pointer-type parsing | `src/parser.cpp:255-262` | `*` token appended to the type string |
| Function pointer types | `src/parser.cpp:527-547` | Handles `void (*)(int)` style declarations |
| Multi-arg call | `src/codegen.cpp:1288-1364` | System V ABI register assignment |
| Address-of codegen | `src/codegen.cpp:1466-1480` | `lea <offset>(%rbp), %rax` |
| String literal codegen | `src/codegen.cpp:1534-1541` | `lea .Lstr_N(%rip), %rax` |

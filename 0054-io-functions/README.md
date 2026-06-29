# Lesson 0054: I/O Functions

## Status: ✅ Complete | Phase: Stdlib Tier A | Effort: Medium

## Objective

Enable programs to call the standard C I/O functions (`printf`,
`scanf`, `puts`, `putchar`, `fopen`, `fclose`, `fread`, `fwrite`, etc.)
by declaring them as `extern` (or `#include`-ing the bundled header)
and letting the linker resolve the calls against the system C library.

## Implementation

`simplecc` ships a minimal bundled header in `lib/stdio.h`:

```c
// lib/stdio.h
#pragma once

#define NULL ((void*)0)
#define EOF (-1)

typedef struct FILE FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

int   printf(const char *fmt, ...);
int   fprintf(FILE *stream, const char *fmt, ...);
int   sprintf(char *str, const char *fmt, ...);
int   snprintf(char *str, int n, const char *fmt, ...);
int   scanf(const char *fmt, ...);
int   sscanf(const char *str, const char *fmt, ...);

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

int   putchar(int c);
int   getchar();
int   puts(const char *s);
int   ungetc(int c, FILE *stream);
```

When a user writes `#include <stdio.h>` the preprocessor reads
`lib/stdio.h` and substitutes the declarations.

## Variadic Argument Support

`printf` and the other variadic `*printf` functions are declared with
`...` (the `ELLIPSIS` token). The parser recognises this in
`parse_param` (`src/parser.cpp:937-942`) and creates a `ParamNode`
named `"..."`. The codegen treats variadic arguments exactly like
fixed arguments — they are passed through `%rdi..%r9` / `%xmm0..%xmm7`
per the System V ABI.

`simplecc` does not generate the `SAVE_ARGS` / `EH_INFO` prologue that
GCC emits in variadic callees. A `simplecc`-compiled `printf` would
misbehave at runtime; a `printf` linked in from the system C library
works correctly because the libc implementation sets up its own
variadic prologue.

## What Works

- Declaring any standard I/O function as `extern` and calling it.
- The bundled `lib/stdio.h` covers the most common functions.
- Format-string literals work normally — they are emitted into
  `.rodata` and loaded with `lea .Lstr_N(%rip), %rax`.

## Limitations

- No implementation of `printf` / `scanf` is bundled — the user must
  link against the system C library.
- The compiler does not implement a variadic callee prologue, so a
  variadic function compiled by `simplecc` cannot be safely called
  with varargs.
- The opaque `FILE *` type is declared as `typedef struct FILE FILE;`
  — the structure is never defined, so the user can only ever pass
  `FILE *` values around as pointers.

## Example

```c
int printf(char *fmt, ...);
int main() {
    printf("hello\n");
    return 0;
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Bundled header | `lib/stdio.h:1-37` | Declarations of the common stdio functions |
| `ELLIPSIS` token | `src/lexer.cpp:484` | Lexer recognises `...` |
| `parse_param` | `src/parser.cpp:937-942` | Creates a variadic `ParamNode` |
| String literal codegen | `src/codegen.cpp:1534-1541` | `lea .Lstr_N(%rip), %rax` |
| Function call codegen | `src/codegen.cpp:1288-1364` | System V ABI register assignment |
| `.rodata` emit | `src/codegen.cpp:78-85` | `.asciz` for each string literal |

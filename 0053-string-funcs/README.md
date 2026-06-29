# Lesson 0053: String Functions

## Status: ✅ Complete | Phase: Stdlib Tier A | Effort: Easy

## Objective

Enable programs to call the standard C string and memory functions
(`strlen`, `strcmp`, `strcpy`, `strcat`, `memcpy`, `memset`, `memmove`,
`strchr`, `strstr`, etc.) by declaring them as `extern` and letting the
linker resolve the calls against the system C library.

## Implementation

`simplecc` ships a minimal bundled header in `lib/string.h` that
declares the common string functions:

```c
// lib/string.h
#pragma once

int   strlen(const char *s);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, int n);
int   strcmp(const char *s1, const char *s2);
int   strncmp(const char *s1, const char *s2, int n);
char *strcat(char *dest, const char *src);
char *strncat(char *dest, const char *src, int n);

void *memcpy(void *dest, const void *src, int n);
void *memmove(void *dest, const void *src, int n);
void *memset(void *s, int c, int n);
int   memcmp(const void *s1, const void *s2, int n);

char *strstr(const char *haystack, const char *needle);
char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);
```

When a user writes `#include <string.h>` the preprocessor reads
`lib/string.h` and substitutes the declarations directly
(`src/preprocessor.cpp:95-130`).

## What Works

- Declaring any of the standard string functions as `extern` and calling
  them.
- Passing `char *` and `void *` arguments — the compiler treats pointer
  types uniformly and emits `lea <label>(%rip), %rax` for string
  literals and `mov <offset>(%rbp), %rax` for local arrays.
- `const`-qualified pointer parameters (ignored at the codegen level —
  pointers are passed the same way regardless of `const`).
- String-literal codegen: string literals are placed in `.rodata`
  (`src/codegen.cpp:78-85`) and loaded with `lea .Lstr_N(%rip), %rax`
  (`src/codegen.cpp:1534-1541`).

## What Does Not Work

- The compiler does not implement any of the string functions in
  user-space. There is no built-in `strlen` and no built-in `memcpy`.
  All such functions must be linked in from the system C library at
  link time.
- Wide-char functions (`wcs*`) are not declared in `lib/string.h`.

## Example

```c
int strlen(char *s) { int i = 0; while (s[i]) i++; return i; }
int main() { return strlen("hello"); }   // returns 5
```

The example in `src/example.c` ships a tiny hand-rolled `strlen` so
that the test is self-contained, but a real program would use
`#include <string.h>` and rely on the system `strlen`.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Bundled header | `lib/string.h:1-18` | Declarations of the common string/memory functions |
| `#include` resolution | `src/preprocessor.cpp:95-130` | Reads `lib/<name>.h` and inlines it |
| String literal codegen | `src/codegen.cpp:1534-1541` | `lea .Lstr_N(%rip), %rax` for string addresses |
| `.rodata` section | `src/codegen.cpp:78-85` | Emits `.asciz` for each string literal |
| Function call codegen | `src/codegen.cpp:1288-1364` | System V ABI register assignment |

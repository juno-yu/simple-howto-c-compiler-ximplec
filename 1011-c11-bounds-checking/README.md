# Lesson 1011: Bounds Checking (C11 Annex K)

## Status: ⚠️ Skeleton | Standard: C11 (Annex K) | Effort: Medium

## Objective

Secure versions of string/memory functions with bounds checking.

## How It Works

The bundled example in `1011-c11-bounds-checking/src/example.c` simply returns `arr[1]` from a 3-element array — there is no `strcpy_s`, `sprintf_s`, or `fopen_s` shim. simplecc ships **no** `<string.h>` `_s` variants and no `<stdio.h>` `_s` variants in `lib/`.

## Functions (aspirational)

| Secure Function | Replaces |
|-----------------|----------|
| `strcpy_s(dest, destsz, src)` | `strcpy` |
| `strncpy_s(dest, destsz, src, count)` | `strncpy` |
| `strcat_s(dest, destsz, src)` | `strcat` |
| `sprintf_s(buf, bufsz, fmt, ...)` | `sprintf` |
| `scanf_s(fmt, ...)` | `scanf` |
| `fopen_s(pFile, filename, mode)` | `fopen` |

## Source Code References

| Component | File | Status |
|-----------|------|--------|
| `<string.h>` | `lib/string.h` | Standard functions only, no `_s` variants |
| `<stdio.h>` | `lib/stdio.h` | Standard functions only, no `_s` variants |

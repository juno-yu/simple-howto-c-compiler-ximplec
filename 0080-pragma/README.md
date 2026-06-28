# Lesson 0080: `#pragma` Directive

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Trivial

## Objective

Recognize and ignore `#pragma` directives. The compiler is not required to understand or act on pragmas — unknown pragmas should be silently dropped.

## Syntax

```c
#pragma once                              // header guard (informational)
#pragma GCC diagnostic ignored "-Wxxx"    // suppress warnings
#pragma pack(1)                            // struct packing (not honored)
```

## Semantics

- `#pragma` is implementation-defined.
- Unknown pragmas must be ignored with no diagnostic.
- Standardized pragmas include `STDC FENV_ACCESS`, `STDC FP_CONTRACT`, `STDC CX_LIMITED_RANGE`.

## Implementation

The preprocessor detects `#pragma` and skips the rest of the line. No codegen or semantic effect.

## Limitations

- **No pragmas are honored.** Including:
  - `#pragma once` — the compiler does not track header identity; users must use traditional `#ifndef` guards.
  - `#pragma pack` — struct layout is fixed; the alignment is not adjusted.
  - `#pragma GCC diagnostic` — warnings are not selectively suppressed.
- `#pragma STDC ...` (C99 standardized pragmas) are also ignored.

## Example

```c
#pragma once
#pragma GCC diagnostic ignored "-Wunused-variable"
int main() {
    return 0;
}
```

## Source Code References

| Component | File | Description |
|-----------|------|-------------|
| Preprocessor | `src/preprocessor.cpp` | Detects `#pragma` and skips to EOL |

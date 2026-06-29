# Lesson 0080: `#pragma` Directive

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Trivial

## Objective

Recognize and ignore `#pragma` directives. The compiler is not
required to understand or act on pragmas — unknown pragmas should
be silently dropped.

## Syntax

```c
#pragma once                              // header guard (informational)
#pragma GCC diagnostic ignored "-Wxxx"    // suppress warnings
#pragma pack(1)                            // struct packing (not honored)
```

## Semantics

- `#pragma` is implementation-defined.
- Unknown pragmas must be ignored with no diagnostic.
- Standardized pragmas include `STDC FENV_ACCESS`, `STDC FP_CONTRACT`,
  `STDC CX_LIMITED_RANGE`.

## Implementation

The preprocessor detects `#pragma` and dispatches to
`handle_pragma`, which is a no-op:

```cpp
// src/preprocessor.cpp:141
} else if (dir_name == "pragma") {
    handle_pragma(dir_args);
}
```

```cpp
// src/preprocessor.cpp:495
void Preprocessor::handle_pragma(const std::string& args) {
    // Ignore pragmas for now
}
```

The directive name and its arguments are consumed but the body of
the pragma (e.g. `GCC diagnostic ignored "-Wxxx"`) is discarded.
No codegen, no semantic effect.

## What Works

- Any `#pragma <anything>` is accepted with no diagnostic.
- The directive is consumed without affecting the rest of the
  translation unit.
- `#pragma once` and `#pragma pack(1)` are accepted the same way
  (and both ignored).

## Limitations

- **No pragmas are honored.** Including:
  - `#pragma once` — the compiler does not track header identity;
    users must use traditional `#ifndef` guards.
  - `#pragma pack` — struct layout is fixed; the alignment is not
    adjusted.
  - `#pragma GCC diagnostic` — warnings are not selectively
    suppressed (and the compiler does not emit most warnings
    anyway).
  - `#pragma STDC ...` (C99 standardized pragmas) are also ignored.
- **No diagnostic** is produced for unrecognized pragmas — they
  are silently dropped.

## Example

```c
#pragma once
#pragma GCC diagnostic ignored "-Wunused-variable"
int main() {
    return 0;
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Directive dispatch | `src/preprocessor.cpp:141-142` | `pragma` case in the directive handler |
| `handle_pragma` | `src/preprocessor.cpp:495-497` | No-op body |

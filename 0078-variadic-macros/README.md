# Lesson 0078: Variadic Macros

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Easy

## Objective

Support C99 variadic macros that accept a variable number of arguments via `...` and `__VA_ARGS__` (or `...` and named arguments in GNU mode).

## Syntax

```c
#define SUM(...)      __VA_ARGS__
#define LOG(msg, ...) fprintf(stderr, msg, __VA_ARGS__)
#define DEBUG(fmt, args...) printf(fmt, args)
```

## Semantics

- `...` in the parameter list matches zero or more arguments.
- `__VA_ARGS__` expands to the comma-separated list of variadic arguments.
- C20 adds `__VA_OPT__(content)` to expand `content` only when `__VA_ARGS__` is non-empty (not implemented in this project).

## Implementation

The preprocessor:
1. Detects `(` immediately after a `#define` identifier and parses a parameter list.
2. Recognizes `...` as a special parameter.
3. During expansion, `__VA_ARGS__` is replaced with the matched arguments (or empty if none).
4. Rescanning handles the case where expanded arguments themselves contain commas.

## Limitations

- `__VA_OPT__` is not implemented — C20 feature.
- GNU-style named variadic args (`args...`) are not recognized; only `__VA_ARGS__` works.

## Example

```c
#define SUM(...) __VA_ARGS__
#define LOG(msg, ...) msg
int main() {
    int x = SUM(1 + 2 + 3);   // expands to: int x = 1 + 2 + 3;
    return x;                  // returns 6
}
```

## Source Code References

| Component | File | Description |
|-----------|------|-------------|
| Macro definition | `src/preprocessor.cpp` | Detects `(` and variadic `...` |
| Macro expansion | `src/preprocessor.cpp` | Replaces `__VA_ARGS__` with args |
| Rescan | `src/preprocessor.cpp` | Re-scans expanded text for nested macros |

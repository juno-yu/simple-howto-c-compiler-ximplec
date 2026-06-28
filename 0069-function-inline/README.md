# Lesson 0069: Function Inlining

## Status: ⚠️ Partial | Phase: Optimization | Effort: Hard

## Objective

Replace function calls with the body of the function when the call site warrants it (small functions, hot paths, marked with `inline` or `__attribute__((always_inline))`).

## Implementation Status

| Feature | Status |
|---------|--------|
| Parse `inline` keyword | ✅ |
| Emit `inline` functions normally | ✅ (no inlining performed) |
| Inline `static` functions at call site | ❌ Not implemented |
| Honor `__attribute__((always_inline))` | ❌ Not implemented |
| Cost model for inlining decisions | ❌ Not implemented |

## Limitation

The compiler recognizes the `inline` keyword (a storage class specifier) but treats `inline` functions identically to non-inline functions: each call site emits a `call` instruction and the linker resolves the name.

## Example

```c
static inline int square(int x) { return x * x; }
int main() { return square(5); }
```

Currently produces:

```asm
call square
```

A proper inliner would replace this with:

```asm
mov $5, %rax
imul %rax, %rax
```

## Future Work

1. AST-level pass that finds call sites.
2. For each call to an `inline` or `static` function with visible body, replace the call expression with a copy of the function body, with parameters substituted for arguments.
3. Handle name mangling for local variables in the inlined body.
4. Honor cost heuristics (function size, call frequency).
5. Recursive inline: mark recursive calls as needing a real function definition.

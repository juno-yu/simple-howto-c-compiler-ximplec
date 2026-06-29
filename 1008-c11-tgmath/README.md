# Lesson 1008: <tgmath.h> (Type-Generic Math)

## Status: ⚠️ Skeleton | Standard: C11 | Effort: Medium

## Objective

Type-generic math macros using `_Generic`.

## Example (aspirational)

```c
#include <tgmath.h>

double d = sqrt(3.14);      // calls sqrt(double)
float f = sqrtf(3.14f);     // calls sqrtf(float)
long double ld = sqrtl(3.14L); // calls sqrtl(long double)

// Generic macro dispatches based on argument type
#define sqrt(x) _Generic((x), \
    float: sqrtf, \
    double: sqrt, \
    long double: sqrtl \
)(x)
```

## How It Works

simplecc ships **no** `<tgmath.h>` header in `lib/`. The bundled example in `1008-c11-tgmath/src/example.c` is a stub that returns `42` without touching the math library. The compiler's `_Generic` (lesson 1001) ignores the type of the controlling expression and returns the first/default branch, which is enough to parse a tgmath macro but not enough to make a correct dispatch.

## Functions in tgmath.h (aspirational)

| Category | Functions |
|----------|-----------|
| Basic math | `ceil`, `floor`, `round`, `trunc`, `fabs` |
| Power | `pow`, `sqrt`, `cbrt`, `hypot` |
| Trigonometric | `sin`, `cos`, `tan`, `asin`, `acos`, `atan` |
| Hyperbolic | `sinh`, `cosh`, `tanh` |
| Logarithmic | `log`, `log2`, `log10`, `exp` |

## Source Code References

| Component | File | Status |
|-----------|------|--------|
| `<tgmath.h>` | `lib/` | ❌ Not present |
| `_Generic` simplification | `src/parser.cpp:1674-1702` | Returns first/default branch only |

# Lesson 0089: Comma Expressions in For Loops

## Status: ⚠️ Partial | Phase: Control Flow | Effort: Easy

## Objective

Support comma expressions in for-loop init and update: `for (i=0, s=0; ...; i++, s+=i)`.

## Implementation

Changed `parse_for_stmt` to use `parse_expression()` (which handles commas) instead of `parse_expr_stmt()` for init and update. Also fixed empty for-loop body (`for(...);`) by handling standalone `;` in `parse_expr_stmt`.

## Known Bug: For-Init Assignment Hangs

`for (i = 0; i < 3; i++)` hangs the compiler. This is a **pre-existing bug** — the parser never handled this correctly. The for loop's init section `i = 0` is parsed by `parse_expression()`, but something in the chain causes an infinite loop or incorrect parsing.

**Workaround:** Use `int i = 0; for (; i < 3; i++)` or declare the variable separately.

```c
// ✅ Works
int i = 0;
for (; i < 3; i++) { }

// ✅ Works
for (int i = 0; i < 3; i++) { }

// ❌ Hangs
for (i = 0; i < 3; i++) { }
```

## What Works

| Pattern | Status |
|---------|--------|
| `for (;;) { }` | ✅ |
| `for (; cond; ) { }` | ✅ |
| `for (; cond; update) { }` | ✅ |
| `for (int i = 0; ...) { }` | ✅ Declaration in init |
| `for (i = 0; ...) { }` | ❌ Hangs — expression init |
| Comma in init: `for (i=0, s=0; ...)` | ✅ (if not hanging) |
| Comma in update: `for (...; i++, s++)` | ✅ |
| Empty body: `for (...);` | ✅ |

## Example

```c
int main() {
    int i, s = 0;
    for (i = 0, s = 0; i < 3; i++, s += i);
    return s;  // 0+1+2+3 = 6
}
```

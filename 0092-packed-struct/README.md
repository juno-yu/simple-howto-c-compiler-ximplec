# Lesson 0092: __attribute__((packed)) Structs

## Status: ⚠️ Partial | Phase: Advanced Types | Effort: Easy

## Objective

Support `__attribute__((packed))` to remove struct padding.

## Status

| Feature | Status |
|---------|--------|
| `__attribute__((packed))` parsed | ✅ |
| Struct layout ignores padding | ⚠️ Still padded |

## Limitation

The packed attribute is parsed but struct layout doesn't remove padding. `sizeof(struct S)` still includes alignment padding.

## Example (expected vs actual)

```c
struct __attribute__((packed)) S { char c; int x; };
// Expected sizeof: 5 (no padding)
// Actual sizeof: 8 (padding preserved)
```
